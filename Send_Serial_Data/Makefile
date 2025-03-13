main: curl.o main.o json.o
	$(CC) curl.o main.o json.o -o main
curl.o: curl.h curl.c
	$(CC) curl.h curl.c -c
json.o: json.h json.c
	$(CC) json.h json.c -c
main.o: curl.h json.h main.c
	$(CC) curl.h json.h main.c -c

clean:
	rm *.o
	rm *.h.gch
	rm main