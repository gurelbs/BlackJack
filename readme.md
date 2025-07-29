# BlackJack Game

A console-based BlackJack game written in C by Gurel Ben Shabat.

## Build Commands

```bash
# Compile the game
make
#run the file
./blackjeck
# or (for windows)
./blackjeck.exe

# Clean build files
make clean
```

### Manual Compilation

```bash
gcc blackjeck.c -Wall -Wextra -std=c99 -pedantic -g -o blackjeck
```

```bash
./blackjeck
```

## Overview

This is a classic BlackJack implementation featuring:

- Standard 52-card deck with 4 suits (Hearts ♥, Clubs ♣, Diamonds ♦, Spades ♠)
- Player vs Dealer gameplay
- Betting system with starting cash of $1000
- Proper BlackJack rules including:
  - Ace handling (1 or 11 value)
  - Dealer hits until 17
  - BlackJack (21 with 2 cards) pays 2.5x
  - Bust detection

## Project Structure

```bash
BlackJack/
├── blackjeck.c      # Main game implementation
├── blackjeck.h      # Header file with structures and function declarations
├── Makefile         # Build configuration
└── readme.md        # This file
```

## Building and Running

### Prerequisites

- GCC compiler
- Standard C library

## Data Structures

### Card

- `uint8_t data`: Encoded card data (rank in lower 4 bits, suit in upper 4 bits)
- `struct Card *next`: Pointer to next card (linked list)

### CardsList

- Linked list implementation for managing cards
- Tracks head, tail, and length

### BlackJackGameState

- Complete game state including deck, player hand, dealer hand
- Cash management and game statistics

## Key Features

- **Random Card Drawing**: Cards are randomly removed from the deck using [`card_remove_at`](blackjeck.c)
- **Smart Ace Handling**: The [`calc_total`](blackjeck.c) function automatically adjusts Ace values
- **Visual Card Display**: ASCII art card representation in [`show_cards`](blackjeck.c)
- **Memory Management**: Proper cleanup with [`deallocate_cards_list`](blackjeck.c)
- **Input Validation**: Robust betting and game input handling

## How to Play

1. **Starting**: You begin with $1000 cash
2. **Betting**: Place bets between $10 and your available cash
3. **Dealing**: You and the dealer each get 2 cards (one dealer card hidden)
4. **Player Turn**: Choose to Hit (take another card) or Stand (keep current total)
5. **Dealer Turn**: Dealer automatically hits until reaching 17 or higher
6. **Winning Conditions**:
   - BlackJack (21 with 2 cards): 2.5x payout
   - Beat dealer without busting: 2x payout
   - Dealer busts: 2x payout
   - Tie: Get your bet back

## Game Rules

- **BlackJack**: 21 points exactly
- **Bust**: Over 21 points (automatic loss)
- **Dealer**: Must hit on 16 or less, stand on 17 or more
- **Aces**: Count as 1 or 11 (automatically optimized)
- **Face Cards**: Jack, Queen, King all count as 10

## Code Highlights

### Card Encoding

Cards use efficient bit packing in the [`Card`](blackjeck.h) structure:

- Lower 4 bits: rank (1-13)
- Upper 4 bits: suit (encoded as powers of 2)

### Memory Safety

- All allocated memory is properly freed in [`cleanup`](blackjeck.c)
- Error handling for malloc failures
- Null pointer checks throughout

### Game Flow

The main game loop in [`game_loop`](blackjeck.c) handles:

1. Betting via [`betting`](blackjeck.c)
2. Initial dealing via [`first_dealing`](blackjeck.c)
3. Player decisions via [`hit_or_stand`](blackjeck.c)
4. Dealer play and resolution

## Constants

Key game constants defined in [blackjeck.h](blackjeck.h):

- `CARDS_DECK_NUM`: 52 cards total
- `BLACKJECK`: 21 points for BlackJack
- `CASH`: Starting cash amount ($1000)
- `RANK_MASK`: Bit mask for extracting card rank

## Author

Gurel Ben Shabat

## License

This project is for educational purposes.
