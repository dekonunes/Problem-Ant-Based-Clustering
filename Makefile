DEBUG=-g
CPP11=-std=c++11

all: ant

main.o: main.cpp
	g++ $(CPP11) $(DEBUG) -c -Wall -pg main.cpp
	
ant: main.o
	g++ main.o -o ant -pthread 

clean:
	rm -rf *.gch *o ant *~; clear;clear;
