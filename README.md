# Streaming Service Phase-A
Streaming Service Simulation, Written in C programming Language, part of Data Structures course

Run using Makefile: <br>
-make <br>
./cs240StreamingService <testfile> <br>
*afterwards* <br>
-make clean <br>
Description: <br>
Users with watch history and suggested movie list<br>
Movies with movie id (mid) and release year (year) as well as movie category<br>
6 Categories (0->5): Horror, SCI-FI, Drama, Romance, Documentary, Comedy <br><br>


Events:
1)  R (Register User) -> (int uid)
       * Adds user with ID uid to
       * users list, as long as no
       * other user with the same uid
       * already exists.

2)  U (Unregister User) (int uid)
       * Removes user with ID uid from
       * users list, as long as such a
       * user exists, after clearing the
       * user's suggested movie list and
       * watch history stack

3)  A (Add new movie) (unsigned mid, enum category, unsigned year)
       * Adds movie with ID mid, category
       * category and release year year
       * to new movies list. The new movies
       * list must remain sorted (increasing
       * order based on movie ID) after every
       * insertion.

4)  D (Distribute new movies) (void)
       * Distributes movies from the new movies
       * list to the per-category sorted movie
       * lists of the category list array. The new
       * movies list should be empty after this
       * event. This event must be implemented in
       * O(n) time complexity, where n is the size
       * of the new movies list

5)  W (Watch Movie) (int uid, unsigned mid)
       * Adds a new struct movie with information
       * corresponding to those of the movie with ID
       * mid to the top of the watch history stack
       * of user uid.

6)  S (Suggest Movie) (int uid)
       * For each user in the users list with
       * id != uid, pops a struct movie from the
       * user's watch history stack, and adds a
       * struct suggested_movie to user uid's
       * suggested movies list in alternating
       * fashion, once from user uid's suggestedHead
       * pointer and following next pointers, and
       * once from user uid's suggestedTail pointer
       * and following prev pointers. This event
       * should be implemented with time complexity
       * O(n), where n is the size of the users list

7)  F (Filtered Movie Search) (int uid, enum category1, enum category2, unsigned year)
       * User uid asks to be suggested movies
       * belonging to either category1 or category2
       * and with release year >= year. The resulting
       * suggested movies list must be sorted with
       * increasing order based on movie ID (as the
       * two category lists). This event should be
       * implemented with time complexity O(n + m),
       * where n, m are the sizes of the two category lists

8)  T (Take Off Movie) (unsigned mid)
       * Movie mid is taken off the service. It is removed
       * from every user's suggested list -if present- and
       * from the corresponding category list.

9)  M (Movie Print) (unsigned mid)
       * Prints information on movies in
       * per-category lists

10)  P (Print User) (void)
       * Prints information on users in
       * users list

Testfile format:
<event code\> <arg1\> <arg2\> etc...
