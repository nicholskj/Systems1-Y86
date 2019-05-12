CC = g++
CFLAGS = -g -c -Wall -std=c++11 -Og
OBJ = Memory.o Tools.o RegisterFile.o ConditionCodes.o lab5.o Loader.o

.C.o:
	$(CC) $(CFLAGS) $< -o $@

lab5: $(OBJ)

Loader.o: Loader.h Memory.h

ConditionCodes.o: ConditionCodes.h

Memory.o: Memory.h

RegisterFile.o: RegisterFile.h

Tools.o: Tools.h

clean:
	rm $(OBJ) lab4

run:
	make clean
	make lab4
	./run.sh

