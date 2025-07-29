CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -pedantic -g
FILES=blackjeck.c blackjeck.h

game: $(FILES)
	$(CC) blackjeck.c $(CFLAGS) -o blackjeck
	
.PHONY: clean run
clean: 
	rm -f blackjeck