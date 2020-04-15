CFLAGS=-g -c -Wall

all: main

main: main.o
	g++ main.o -g -o main

main.o: main.cpp
	g++ $(CFLAGS) main.cpp
