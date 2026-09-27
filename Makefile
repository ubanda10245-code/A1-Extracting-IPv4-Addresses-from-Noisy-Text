CC = gcc
CFLAGS = -Wall

all: main

main: src/main.c
	$(CC) $(CFLAGS) src/main.c -o main

clean:
	rm -f src/main main.exe

.PHONY: all clean