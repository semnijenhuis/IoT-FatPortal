LIB_NAME = modbus

readModbus: readModbus.o main.o
	$(CC) readModbus.o main.o -l$(LIB_NAME) -o readModbus
main.o: readModbus.h main.c
	$(CC) readModbus.h main.c -c
readModbus.o: readModbus.c
	$(CC) readModbus.h readModbus.c -c

clean:
	rm *.o
	rm *.h.gch
	rm readModbus