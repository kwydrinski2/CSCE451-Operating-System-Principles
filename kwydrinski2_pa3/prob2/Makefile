CC = gcc
CFLAGS = -Wall -g

.PHONY: all
all: part2

part2: part2.o
	$(CC) $(CFLAGS) -o $@ $^

part2.o: part2.c monitor.c

.PHONY: clean
clean:
	rm -rf part2 *.o


