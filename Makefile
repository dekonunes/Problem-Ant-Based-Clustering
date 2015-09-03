SFML_LIB=-L./SFML-2.3.1/lib -lsfml-graphics -lsfml-window -lsfml-system
SFML_INCLUDE=-I./SFML-2.3.1/include
DEBUG=-g
CPP11=-std=c++11

all: ant

main.o: main.cpp
	g++ $(CPP11) $(DEBUG) -c -Wall main.cpp $(SFML_INCLUDE)
	
ant: main.o
	g++ main.o -o ant -pthread $(SFML_LIB)

clean:
	rm -rf *.gch *o ant *~; clear;clear;
