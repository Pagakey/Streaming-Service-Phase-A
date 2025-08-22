CC=gcc
CFLAGS=-ansi -g

cs240StreamingService: main.o streaming_service.o
	$(CC) $(CFLAGS) $^ -o $@

main.o: main.c streaming_service.h
	$(CC) $(CFLAGS) -c $< -o $@

streaming_service.o: streaming_service.c streaming_service.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f cs240StreamingService *.o
