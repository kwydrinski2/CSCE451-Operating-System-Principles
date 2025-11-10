CC = gcc
CFLAGS = -Wall -g

.PHONY: all
all: part1

part1: part1.o
	$(CC) $(CFLAGS) -o $@ $^

part1.o: part1.c

.PHONY: clean
clean: 
	rm -rf part1 *.o
