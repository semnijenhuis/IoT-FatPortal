ARCH := MIPS

readSerial: saveData.o readSerial.o main.o
	${CC} saveData.o readSerial.o main.o -o readSerial
main.o: readSerial.h main.c
	${CC} readSerial.h main.c -c
readSerial.o: readSerial.h readSerial.c
	${CC} readSerial.h readSerial.c -c
saveData.o: saveData.h saveData.c
	${CC} saveData.h saveData.c -c

clean:
	rm *.o
	rm *.h.gch
	rm readSerial
