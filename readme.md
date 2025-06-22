#include <ncurses.h>  // TODO: add to read me instruction for install: sudo apt install libncurses5-dev libncursesw5-dev


# Black-Jack Game

## This is a simple implementation of a Black-Jack game using C programming language.

## Features
- Single player mode
- Dealer AI
- Basic betting system
- Card shuffling and dealing
- Game loop with win/loss conditions
## How to Run

1. Make sure you have a C compiler installed (like GCC).
2. Clone the repository or download the source code.
3. Open a terminal and navigate to the directory containing the source code.

4. Compile the code using the command:
   ```bash
   gcc -o blackjack blackjack.c
   ```

5. Run the compiled program:
   ```bash
    ./blackjack
    ```
6. Follow the on-screen instructions to play the game.
## Game Rules
- The player starts with a certain amount of money.
- The player can place a bet before each round.
- The player and dealer are each dealt two cards.
- The player can choose to "hit" (take another card) or "stand" (end their turn).
- The goal is to get as close to 21 as possible without going over.
- If the player's total exceeds 21, they lose.
- If the player has a higher total than the dealer without exceeding 21, they win.
- If the dealer exceeds 21, the player wins.
- If both the player and dealer have the same total, it's a push (no one wins).
## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
## Contributing
Contributions are welcome! If you have suggestions for improvements or new features, feel free to open an issue or submit a pull request.
## Contact
For any questions or feedback, please contact the project maintainer at [


### game initialization

card is uint8_t - 8 bits, the first 4 bits are the suit, the last 4 bits are the value (linked list)

