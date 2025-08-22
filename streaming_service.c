#include <stdio.h>
#include <stdlib.h>
#include "streaming_service.h"


User_PTR UserListHead;
User_PTR LastUserInserted;
New_movie_PTR New_MovieList_head;

User_PTR sentinel;

Movie_PTR HOR_head;
Movie_PTR SCI_head;
Movie_PTR DRM_head;
Movie_PTR ROM_head;
Movie_PTR DOC_head;
Movie_PTR COM_head;


Movie_PTR *MovieTable[6];

int register_user(int uid){
    User_PTR tmp = UserListHead;
    if(IfUserAlreadyExists(uid))
        return -1;
    else{
        if(tmp->uid==-1){
            RegisterUser_start(uid);
        }else{
            RegisterUser_after(uid);
        }
        printf("\nR<%d>\n\tUsers = ",uid);
        FakePrint();
        return 0;
    }
}

/*U event done*/
void unregister_user(int uid) {
    User_PTR prev_ptr = NULL;
    User_PTR tmp = UserListHead;
    User_PTR found_user = NULL;

    if(tmp->uid==-1)
        return;
    found_user = SearchUser(uid);
    ClearSuggestionList(found_user);
    ClearWatchHistory(found_user);
    if(found_user!=NULL){
        if(UserListHead==found_user){
            UserListHead = found_user->next;
            found_user->next = NULL;
            free(found_user);
        }else{
            while(tmp!=found_user){
                prev_ptr = tmp;
                tmp = tmp->next;
            }
            prev_ptr->next = tmp->next;
            tmp->next = NULL;

        }
    }
    printf("\nU<%d>\n\tUsers = ",uid);
    FakePrint();
}

/*WORKS*/
int add_new_movie(unsigned mid, movieCategory_t category, unsigned year) {
    New_movie_PTR curr = New_MovieList_head;
    New_movie_PTR prev = NULL;

    while (curr != NULL && curr->info.mid < mid) {
        prev = curr;
        curr = curr->next;
    }

    New_movie_PTR newmovie = CreateNewMovie(mid, category, year);
    if (!newmovie) {
        printf("Failed to add new movie with ID: %u due to memory allocation failure.\n", mid);
        return FALSE;
    }
    if (prev == NULL) {
        InsertNewMovie_Start(newmovie);
    } else {
        InsertNewMovie_After(prev,newmovie);
    }
    printf("\nA <%u> <%s> <%u>\n\tNew movies = ",mid, GetCategoryName(category),year);
    PrintAddNewMovie();
    return TRUE;
}

void PrintAddNewMovie(){
    New_movie_PTR tmp = New_MovieList_head;
    if(New_MovieList_head==NULL)
        return;
    while(tmp!=NULL){
        printf("<mid_%u,category_%s,year_%u>",tmp->info.mid, GetCategoryName(tmp->category),tmp->info.year);
        if(tmp->next!=NULL)
            printf(", ");
        tmp = tmp->next;
    }
    printf("\nDONE\n");
}

char* GetCategoryName(movieCategory_t movieCategory){
    switch(movieCategory){
        case HORROR:return "HORROR";
        case COMEDY:return "COMEDY";
        case DRAMA:return "DRAMA";
        case DOCUMENTARY:return "DOCUMENTARY";
        case ROMANCE:return "ROMANCE";
        case SCIFI:return "SCI-FI";
    }
}


void print_moviesD() {
    Movie_PTR curr = NULL;
    printf("\nD\nCategorized Movies: \n");
    int i;
    for (i = 0; i < 6; i++) {
        if (MovieTable[i] == NULL) {
            continue;
        }
        curr = *MovieTable[i];
        switch (i) {
            case 0:
                printf("\tHorror: ");
                break;
            case 1:
                printf("\tSci-Fi: ");
                break;
            case 2:
                printf("\tDrama: ");
                break;
            case 3:
                printf("\tRomance: ");
                break;
            case 4:
                printf("\tDocumentary: ");
                break;
            case 5:
                printf("\tComedy: ");
                break;
        }
        if (curr == NULL) {
            printf("None\n");
            continue;
        }

        while (curr != NULL) {
            printf("<mid_%d,%u>", i, curr->info.mid);
            if (curr->next != NULL) {
                printf(", ");
            }
            curr = curr->next;
        }
        printf("\n");
    }
    printf("\nDONE\n");
}


/*M event*/
void print_movies() {
    Movie_PTR curr = NULL;
    printf("\nM\nCategorized Movies: \n");
    int i;
    int cnt;
    for (i = 0; i < 6; i++) {
        cnt = 1;
        if (MovieTable[i] == NULL) {
            continue;
        }
        curr = *MovieTable[i];
        switch (i) {
            case 0:
                printf("\tHorror: ");
                break;
            case 1:
                printf("\tSci-Fi: ");
                break;
            case 2:
                printf("\tDrama: ");
                break;
            case 3:
                printf("\tRomance: ");
                break;
            case 4:
                printf("\tDocumentary: ");
                break;
            case 5:
                printf("\tComedy: ");
                break;
        }
        if (curr == NULL) {
            printf("None\n");
            continue;
        }

        while (curr != NULL) {
            printf("<mid_%u,%d>",curr->info.mid,cnt);
            if (curr->next != NULL) {
                printf(", ");
            }
            cnt++;
            curr = curr->next;
        }
        printf("\n");
    }
    printf("\nDONE\n");
}
/*D event*/
void distribute_new_movies(void){

    /*current new movie*/
    New_movie_PTR cnm = NULL;
    Movie_PTR *category_head = NULL;
    Movie_PTR tmp = NULL;

    while((cnm = Remove_New_Movie())!=NULL) {
        Movie_PTR newmovie = (Movie_PTR) malloc(sizeof(Movie));
        if (!newmovie) {
            printf("DistributeNewMovies. Malloc Failure!!!");
            break;
        }
        newmovie->info.mid = cnm->info.mid;
        newmovie->info.year = cnm->info.year;
        newmovie->next = NULL;
        category_head = MovieTable[cnm->category];
        if (*category_head == NULL)
            *category_head = newmovie;
        else {
            tmp = *category_head;
            while (tmp->next != NULL)
                tmp = tmp->next;
            tmp->next = newmovie;
        }
        free(cnm);
    }
    print_moviesD();
}


int IfUserAlreadyExists(int uid){
    User_PTR tmp = SearchUser(uid);
    if(tmp==NULL)
        return FALSE;
    else
        return TRUE;
}

User_PTR Create_User(int uid){
    User_PTR newuser = (User_PTR)malloc(sizeof(User));
    if(!newuser){
        printf("CreateUser. Malloc Failure!!!");
        return NULL;
    }
    newuser->uid = uid;
    newuser->next = NULL;
    newuser->suggestedHead = NULL;
    newuser->suggestedTail = NULL;
    newuser->watchHistory = NULL;
    return newuser;
}

int RegisterUser_start(int uid){
    User_PTR newuser = Create_User(uid);
    if(newuser!=NULL){
        newuser->next = UserListHead;
        UserListHead = newuser;
        LastUserInserted = newuser;
        return TRUE;
    }else{
        free(newuser);
        return FALSE;
    }
}

int RegisterUser_after(int uid){
    User_PTR newuser = Create_User(uid);
    if(newuser!=NULL){
        newuser->next = LastUserInserted->next;
        LastUserInserted->next = newuser;
        LastUserInserted = newuser;
        return TRUE;
    }else{
        free(newuser);
        return FALSE;
    }
}

void ClearSuggestionList(User_PTR d){
    SugMovie_PTR ptr;
    if(d->uid==-1 || d==NULL)
        return;
    while(d->suggestedHead!=NULL){
        ptr = d->suggestedHead;
        d->suggestedHead = d->suggestedHead->next;
        free(ptr);
    }
    d->suggestedHead = NULL;
    d->suggestedTail = NULL;
}

void ClearWatchHistory(User_PTR d){
    Movie_PTR ptr;
    if(d==NULL)
        return;
    while(d->watchHistory!=NULL){
        ptr = d->watchHistory;
        d->watchHistory = d->watchHistory->next;
        free(ptr);
    }
    d->watchHistory = NULL;
}

New_movie_PTR CreateNewMovie(unsigned mid, movieCategory_t category, unsigned year){
    New_movie_PTR newmovie = (New_movie_PTR)malloc(sizeof(New_movie));
    if(!newmovie){
        printf("CreateNewMovie. Malloc Failure!!!");
        return NULL;
    }
    newmovie->info.mid = mid;
    newmovie->info.year = year;
    newmovie->category = category;
    newmovie->next = NULL;
    return newmovie;
}

int InsertNewMovie_Start(New_movie_PTR newmovie){
    if(newmovie!=NULL){
        newmovie->next = New_MovieList_head;
        New_MovieList_head = newmovie;
        return TRUE;
    }
    return FALSE;
}

int InsertNewMovie_After(New_movie_PTR p,New_movie_PTR newmovie){
    if(p!=NULL && newmovie!=NULL){
        newmovie->next = p->next;
        p->next = newmovie;
        return TRUE;
    }
    return FALSE;
}
/*SWSTH EINAI*/
void FakePrint(){
    User_PTR curr = NULL;
    curr = UserListHead;
    if(curr!=NULL){
        while(curr->uid!=-1){
            printf("<uid_%d>",curr->uid);
            if(curr->next->uid!=-1)
                printf(", ");
            curr=curr->next;
        }
        printf("\nDONE\n");
    }
}

New_movie_PTR Remove_New_Movie(){
    New_movie_PTR curr;
    curr = New_MovieList_head;
    if(curr==NULL)
        return NULL;
    New_MovieList_head = New_MovieList_head->next;
    return curr;
}
/*WORKS*/
Movie_PTR PopMovie(int uid){
    User_PTR tmp = UserListHead;
    Movie_PTR moviepopped = NULL;
    if(uid==-1 || tmp->uid==-1)
        return NULL;
    while(tmp->uid!=-1){
        if(tmp->uid==uid){
            if(!IsEmpty(tmp->watchHistory)){
                moviepopped = tmp->watchHistory;
                /*Top moves "down" one movie*/
                tmp->watchHistory=tmp->watchHistory->next;
                moviepopped->next = NULL;
                return moviepopped;
            }else{
                return NULL;
            }
        }
        tmp = tmp->next;
    }
    return NULL;
}

int IsEmpty(Movie_PTR head){
    return head==NULL;
}

Movie_PTR SearchInfo(int k,unsigned mid){
    Movie_PTR tmp;
    tmp = *MovieTable[k];
    if(tmp==NULL){
        return NULL;
    }
    while(tmp!=NULL){
        if(tmp->info.mid==mid)
            return tmp;
        tmp=tmp->next;
    }
    return NULL;
}

void PrintWatchHistory(Movie_PTR head, int uid){
    Movie_PTR tmp = head;
    if(head==NULL){
        return;
    }
    while(tmp!=NULL){
        printf("<mid_%u>",tmp->info.mid);
        if(tmp->next!=NULL)
            printf(", ");
        tmp = tmp->next;
    }
    printf("\nDONE\n");

}
/*W Event*/
int watch_movie(int uid, unsigned mid){
    int i;
    Movie_PTR found_movie = NULL;
    User_PTR found_user = NULL;
    Movie_PTR movie_copy = NULL;
    found_user = SearchUser(uid);
    if(found_user == NULL)
        return FALSE;

    for(i=0;i<6;i++){
        found_movie = SearchInfo(i,mid);
        if(found_movie!=NULL)
            break;
    }

    if(found_movie == NULL)
        return FALSE;

    movie_copy = (Movie_PTR)malloc(sizeof(Movie));
    if (!movie_copy) {
        printf("Failed to allocate memory for watch history.\n");
        return FALSE;
    }
    movie_copy->info.mid = found_movie->info.mid;
    movie_copy->info.year = found_movie->info.year;
    /**movie_copy = *found_movie;*/
    /*Insert at the start since the watchHistory behaves like a stack.*/
    movie_copy->next = found_user->watchHistory;
    found_user->watchHistory = movie_copy;
    printf("W <%d> <%u>\n",uid,mid);
    printf("\tUser <%d> Watch History = ",uid);
    PrintWatchHistory(found_user->watchHistory,uid);
    return TRUE;
}

void CopyMovietoSugMovie(SugMovie_PTR sm, Movie_PTR m){
    sm->info.mid = m->info.mid;
    sm->info.year = m->info.year;
    sm->next = NULL;
    sm->prev = NULL;
}

int suggest_movies(int uid){
    User_PTR user = SearchUser(uid);
    User_PTR tmp = UserListHead;
    Movie_PTR moviepopped = NULL;
    SugMovie_PTR sug_movie = NULL;
    SugMovie_PTR last_movie_head = NULL;
    SugMovie_PTR last_movie_tail = NULL;
    int cnt = 0;
    if(user==NULL){
        printf("User Not Found.Cannot suggest movies...");
        return FALSE;
    }
    if(UserListHead==NULL){
        printf("Empty User List");
        return FALSE;
    }
    while(tmp!=NULL && tmp->uid!=-1){
        moviepopped = PopMovie(tmp->uid);
        if(moviepopped==NULL){
            free(sug_movie);
            return FALSE;
        }
        if(tmp->next==NULL){
            if(tmp->uid==uid || moviepopped==NULL){
                tmp = tmp->next;
                continue;
            }
        }

        sug_movie = (SugMovie_PTR)malloc(sizeof(SugMovie));
        if(!sug_movie){
            printf("Adunamia Desmeushs mnhmhs: suggest_movies");
            return FALSE;
        }
        /*Works*/
        CopyMovietoSugMovie(sug_movie,moviepopped);

        /*Zugos arithmos -> tha mpei sto suggested head*/
        if(cnt%2==0){
            if(user->suggestedHead==NULL){
                user->suggestedHead = sug_movie;
                last_movie_head = sug_movie;
            }else{
                sug_movie->next = last_movie_head->next;
                last_movie_head->next = sug_movie;
                sug_movie->prev = last_movie_head;
                last_movie_head = sug_movie;
            }
        }else{
            if(user->suggestedTail==NULL){
                user->suggestedTail = sug_movie;
                last_movie_tail = user->suggestedTail;
            }else{
                sug_movie->prev = last_movie_tail->prev;
                last_movie_tail->prev = sug_movie;
                sug_movie->next = last_movie_tail;
                last_movie_tail = sug_movie;
            }
        }
        cnt++;
        tmp = tmp->next;
    }
    if (last_movie_head != NULL && last_movie_tail != NULL) {
        last_movie_head->next = last_movie_tail;
        last_movie_tail->prev = last_movie_head;
    } else if (last_movie_head != NULL) {
        user->suggestedTail = last_movie_head;
    }
    printf("\nS <%d>\n",uid);
    PrintSuggestedMovies(user);
    return 0;
}

void PrintSuggestedMovies(User_PTR user){
    SugMovie_PTR tmp = NULL;
    if(user->uid==-1)
        return;
    tmp = user->suggestedHead;
    printf("\tUser <%d> Suggested Movies = ",user->uid);
    while(tmp!=NULL){
        printf("<mid_%u>",tmp->info.mid);
        if(tmp->next!=NULL)
            printf(", ");
        tmp = tmp->next;
    }
    printf("\nDONE\n");
}

int filtered_movie_search(int uid,movieCategory_t category1,movieCategory_t category2, unsigned year){
    Movie_PTR head_cat1 = *MovieTable[category1];
    Movie_PTR head_cat2 = *MovieTable[category2];
    User_PTR user = SearchUser(uid);
    SugMovie_PTR new_dll = NULL;
    SugMovie_PTR movie_to_add;
    SugMovie_PTR last_movie = NULL;
    SugMovie_PTR tmp = NULL;

    if(user==NULL)
        return -1;
    while(head_cat1 != NULL || head_cat2 != NULL) {
        movie_to_add = NULL;
        if ((head_cat1 != NULL && head_cat2 == NULL) || (head_cat1!=NULL && head_cat1->info.mid < head_cat2->info.mid)) {
            if (head_cat1->info.year >= year) {
                movie_to_add = (SugMovie_PTR) malloc(sizeof(SugMovie));
                if(!movie_to_add){
                    printf("Malloc failure!");
                    return FALSE;
                }
                CopyMovietoSugMovie(movie_to_add, head_cat1);
            }
            head_cat1 = head_cat1->next;
        } else if (head_cat2 != NULL) {
            if (head_cat2->info.year >= year) {
                movie_to_add = (SugMovie_PTR) malloc(sizeof(SugMovie));
                if(!movie_to_add){
                    printf("Malloc failure!");
                    return FALSE;
                }
                CopyMovietoSugMovie(movie_to_add, head_cat2);
            }
            head_cat2 = head_cat2->next;
        }
        if (movie_to_add != NULL) {
            movie_to_add->next = NULL;
            if (new_dll == NULL) {
                new_dll = movie_to_add;
                movie_to_add->prev = NULL;
                last_movie = movie_to_add;
            } else {
                movie_to_add->prev = last_movie;
                last_movie->next = movie_to_add;
                last_movie = movie_to_add;
            }
        }
    }

    tmp = new_dll;
    printf("dll contents: ");
    while(tmp!=NULL){
        printf("<%u-%u>,",tmp->info.mid,tmp->info.year);
        tmp = tmp->next;
    }
    /*Anagastika kai to Tail einai NULL efoson me to pou ginei kati suggest, mpainei sto head*/
    if(user->suggestedHead==NULL){
        user->suggestedHead = new_dll;
        user->suggestedHead->prev = NULL;
        user->suggestedTail = last_movie;
        user->suggestedTail->next = NULL;
    }else{
        if(user->suggestedTail!=NULL && user->suggestedHead!=NULL && new_dll!=NULL){
            user->suggestedTail->next = new_dll;
            new_dll->prev = user->suggestedTail;
            user->suggestedTail = last_movie;
            last_movie->next = NULL;
        }

    }
    if(last_movie!=NULL)
        user->suggestedTail = last_movie;
    printf("\nF <%d> <%s> <%s> <%u>\n",uid, GetCategoryName(category1), GetCategoryName(category2),year);
    PrintSuggestedMovies(user);
    return 0;
}

void take_off_movie(unsigned mid){
    Movie_PTR moviePtr = NULL;
    Movie_PTR prevMP = NULL;
    SugMovie_PTR sugMoviePtr = NULL;
    SugMovie_PTR sugPrev = NULL;
    User_PTR user = UserListHead;
    int i;
    printf("\nT <%u>\n",mid);
    /*Removal from every suggestion list of every user*/
    while(user!=NULL && user->uid!=-1){
        /*pointer pointing to the first movie in the current user's suggestion list.*/
        sugMoviePtr = user->suggestedHead;
        sugPrev = NULL;
        /*if the user's suggestion list isnt empty->*/
        while(sugMoviePtr != NULL){
            /*if the desired movie to take_off is found then->*/
            if(sugMoviePtr->info.mid == mid){
                /*if the desired movie is the first one in the list*/
                if(sugMoviePtr==user->suggestedHead){
                    /*The current movie's address is kept on the sugMoviePtr pointer.*/
                    if(user->suggestedHead->next!=NULL)
                        user->suggestedHead = user->suggestedHead->next;
                    if(user->suggestedHead != NULL)
                        user->suggestedHead->prev = NULL;
                    break;
                }else{
                    sugPrev->next = sugMoviePtr->next;
                    if(sugMoviePtr->next != NULL)
                        sugMoviePtr->next->prev = sugPrev;
                }
                printf("\t<%u> removed from <uid_%d> suggested list.\n",mid,user->uid);
                free(sugMoviePtr);
                break;
            }
            sugPrev = sugMoviePtr;
            sugMoviePtr = sugMoviePtr->next;
        }
        user = user->next;
    }

    /*Removal from the Category Table*/
    for(i=0;i<6;i++){
        moviePtr = *MovieTable[i];
        prevMP = NULL;
        while(moviePtr!=NULL){
            if(moviePtr->info.mid == mid){
                if(prevMP == NULL){
                    *MovieTable[i] = moviePtr->next;
                }else{
                    prevMP->next = moviePtr->next;
                }
                printf("\t<%u> removed from <%s> category list.\n",mid, GetCategoryName(i));
                PrintMoviesT(i);
                free(moviePtr);
                break;
            }
            prevMP = moviePtr;
            moviePtr = moviePtr->next;
        }
    }
    printf("\nDONE\n");
}

void PrintMoviesT(int i) {
    Movie_PTR curr = *MovieTable[i];
    int cnt = 1;
    printf("\n\t%s list: ", GetCategoryName(i));
    if (curr == NULL) {
        printf("Empty\n");
        return;
    }
    while (curr != NULL) {
        printf("<mid_%u,%d>",curr->info.mid,cnt);
        if (curr->next != NULL) {
            printf(", ");
        }
        cnt++;
        curr = curr->next;
    }
}
/*SWSTH EINAI*/
User_PTR SearchUser(int uid){
    User_PTR tmp = UserListHead;
    while(tmp->uid!=-1){
        if(tmp->uid==uid)
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

void print_users(void){
    User_PTR users = UserListHead;
    SugMovie_PTR suggested_movies = NULL;
    Movie_PTR watch_history = NULL;
    int smcnt;
    int wlmcnt;
    printf("P\nUsers: ");
    while(users->uid!=-1){
        smcnt = 1;
        wlmcnt = 1;
        suggested_movies = users->suggestedHead;
        watch_history = users->watchHistory;
        printf("\n\t<uid_%d>:\n",users->uid);
        printf("\t\tSuggested: ");
        while(suggested_movies!=NULL){
            printf("<mid_%u,%d>",suggested_movies->info.mid,smcnt);
            if(suggested_movies->next!=NULL)
                printf(", ");
            suggested_movies = suggested_movies->next;
            smcnt++;
        }
        printf("\n\t\tWatch History: ");
        while(watch_history!=NULL){
            printf("<mid'_%u,%d>",watch_history->info.mid,wlmcnt);
            if(watch_history->next!=NULL)
                printf(", ");
            watch_history = watch_history->next;
            wlmcnt++;
        }
        users = users->next;
    }
    printf("\nDONE\n");
}