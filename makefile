# makefile

all: seamTest

functions.o: functions.cpp 
	g++ -std=c++11 -c -g functions.cpp

seamcarving.o : seamcarving.cpp
	g++ -std=c++11 -c -g seamcarving.cpp


seamTest: seamcarving.o functions.o
	g++ -std=c++11 -o seamTest seamcarving.o functions.o

clean:
	rm *.o