CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -pedantic -g
FILES=main.c main.h

game: $(FILES)
	$(CC) main.c $(CFLAGS) -o game

run: game
	./game

.PHONY: clean run
clean: 
	rm -f game