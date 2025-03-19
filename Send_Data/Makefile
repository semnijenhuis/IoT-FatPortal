sendData: readData.o curl.o main.o json.o
	$(CC) readData.o curl.o main.o json.o -o sendData
main.o: curl.h json.h main.c
	$(CC) curl.h json.h main.c -c
curl.o: curl.h curl.c
	$(CC) curl.h curl.c -c
json.o: json.h json.c
	$(CC) json.h json.c -c
readData.o: readData.h readData.c
	$(CC) readData.h readData.c -c

clean:
	rm *.o
	rm *.h.gch
	rm sendData