# makefile for MP6
# Christian Walker
# ECE223 Spring 2014

lab6 : graph.o lab6.o
	gcc -Wall -g graph.o lab6.o -o lab6 -lm

graph.o: graph.c graph.h
	gcc -Wall -g -c graph.c

lab6.o: lab6.c graph.h
	gcc -Wall -g -c lab6.c

clean:
	rm -f *.o lab6 core
