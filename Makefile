CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -pedantic
FILES=main.c main.h

game: $(FILES)
	$(CC) $(FILES) $(CFLAGS) -o game && ./game
.PHONY: 
	clean
clean: 
	rm -f game