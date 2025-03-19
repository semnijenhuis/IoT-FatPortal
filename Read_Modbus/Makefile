LIB_NAME = modbus
LIBS = ../libs
INCLUDE_FILES = ../include

readModbus: readModbus.o saveData.o main.o
	$(CC) readModbus.o main.o saveData.o -l$(LIB_NAME) -L$(LIBS) -o readModbus
main.o: readModbus.h main.c
	$(CC) readModbus.h main.c -c
readModbus.o: readModbus.c
	$(CC) readModbus.h readModbus.c -I$(INCLUDE_FILES) -c 
saveData.o: saveData.c
	$(CC) saveData.h saveData.c -c

clean:
	rm *.o
	rm *.h.gch
	rm readModbus