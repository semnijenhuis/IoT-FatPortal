main: curl.o main.o
	$(CC) curl.o main.o -o main
curl.o: curl.h curl.c
	$(CC) curl.h curl.c -c
main.o: curl.h main.c
	$(CC) curl.h main.c -c