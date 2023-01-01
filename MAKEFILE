CC = gcc
CXX = CC
CCFLAGS = -Wall -Wextra -Wconversion -g -O1

all: main test

%.o : %c
	$(CC) -c $(CCFLAGS) $<

main: main.o
	$(CC) -o main main.o

test: main
	./main

clean:
	rm -f core *.o main
