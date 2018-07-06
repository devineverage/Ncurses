all: sam0 

sam0: sam0.o
	g++ -Wall -lncurses -g -o sam0 sam0.o -lmenu -lncurses


sam0.o: sam0.cpp
	g++ -Wall -std=c++14 -c -lncurses -g -O0 -pedantic-errors sam0.cpp
 
clean:
	rm -rf *o sam0