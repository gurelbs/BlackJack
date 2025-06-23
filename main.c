
#include "screen.h"
#include <inttypes.h>
#include <ncurses.h>  // TODO: add to read me instruction for install: sudo apt install libncurses5-dev libncursesw5-dev
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void init_ui() {
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  srand(time(NULL));
}

void shutdown_ui() { endwin(); }

void display_game(WINDOW *win) {
  werase(win);
  box(win, 0, 0);
  mvwprintw(win, 1, 2, " BLACKJACK TERMINAL EDITION - By Gurel Ben Shabat ");
  mvwprintw(win, 5, 2, " are you ready to play? ");
  mvwprintw(win, 6, 2, "[Y] Yes, Start  [N] No, Quit\n");
  wrefresh(win);
}

char get_user_action(WINDOW *win) { return wgetch(win); }

void game_screen_end(void) {
  endwin();  // End ncurses mode
}

int game_screen_print(const char *message) {
  if (!message) return -1;  // Check for null pointer
  clear();                  // Clear the screen
  printw("%s", message);    // Print the message
  refresh();                // Refresh the screen to show changes
  return 0;                 // Return success
}

void game_screen_main(WINDOW *win, const BlackJackGameState *game) {
  werase(win);
  box(win, 0, 0);

  mvwprintw(win, 2, 2, "=== Current Game Status ===");
  mvwprintw(win, 3, 2, "Cash:         $%d", game->cash);
  mvwprintw(win, 4, 2, "Pot:          $%d", game->pot);
  mvwprintw(win, 5, 2, "Deck size:    %zu cards", game->deck.len);
  mvwprintw(win, 6, 2, "Dealer hand:  %zu cards", game->dealer_hand.len);
  mvwprintw(win, 7, 2, "Player hand:  %zu cards", game->player_hand.len);
  mvwprintw(win, 8, 2, "\n===========================\n");

  wrefresh(win);
}

int cards_list_init(CardsList *list) {
  if (!list) return -1;
  list->head = NULL;
  list->tail = NULL;
  list->len = 0;
  return 0;
}

Card *create_card(unsigned int rank, unsigned int suit) {
  struct Card *new_card = malloc(sizeof(struct Card));
  if (!new_card) return NULL;
  new_card->data = (rank << 4) | suit;
  new_card->next = NULL;
  return new_card;
}

void append_card(CardsList *list, Card *card) {
  if (!list || !card) return;
  if (list->head == NULL) {
    list->head = card;
    list->tail = card;
  } else {
    list->tail->next = card;
    list->tail = card;
  }
  list->len++;
}

void print_binary_card_data(uint8_t value) {
  for (int b = 7; b >= 0; b--) {
    printf("%d", (value >> b) & 1);
  }
}

void print_text_card_data(Card *card_ptr) {
  if (card_ptr->data & HEART) {
    printf(" | HEART");
  } else if (card_ptr->data & CLUBS) {
    printf(" | CLUBS");
  } else if (card_ptr->data & DIAMONDS) {
    printf(" | DIAMONDS");
  } else if (card_ptr->data & SPADES) {
    printf(" | SPADES");
  } else {
    printf(" | ERROR");
  }
}

void fill_deck(CardsList *deck) {
  if (!deck) return;
  int suits[] = {HEART, CLUBS, DIAMONDS, SPADES};
  size_t suits_num = sizeof(suits) / sizeof(suits[0]);
  size_t ranks_num = CARDS_DECK_NUM / suits_num;

  for (size_t i = 0, card_num = 1; i < suits_num; i++) {
    for (size_t rank = 1; rank <= ranks_num; rank++, card_num++) {
      Card *new_card = create_card(rank, suits[i]);
      if (new_card) {
        append_card(deck, new_card);
        // printf("%2zu)\tBinary: ", card_num);
        // print_binary_card_data(new_card->data);
        // print_text_card_data(new_card);
        // printf(" | hex: 0x%02X", new_card->data);
        // puts("");
      }
    }
  }
}

int game_state_init(struct BlackJackGameState *game) {
  if (!game) return -1;

  cards_list_init(&game->deck);
  cards_list_init(&game->dealer_hand);
  cards_list_init(&game->player_hand);
  fill_deck(&game->deck);
  game->cash = 1000;
  game->pot = 0;
  return 0;
}

// calc card rank()

// 1. Game Initialization

int initialization(WINDOW *win, BlackJackGameState *game) {
  char userAnswer;
  // ("\nWellcome to my black jack game!!\n");
  while (true) {
    printw("\nare you ready to play?\tclick Y to YES or N to NO:\t");
    scanf(" %c", &userAnswer);
    while (getchar() != '\n' && getchar() != EOF);
    if (userAnswer == 'Y') {
      game_screen_print("\nOK! lets start the game!\ninitializing...\n\n");
      game_state_init(game);
      printf("Game initialized successfully!\n");
      // // Uncomment and replace the debug prints
      // printw("=== Current Game Status ===\n");
      // printw("\n\n");
      // printw("Cash: $%d\n", game->cash);
      // printw("Pot: $%d\n", game->pot);
      // printw("Deck size: %zu cards\n", game->deck.len);
      // printw("Dealer hand: %zu cards\n", game->dealer_hand.len);
      // printw("Player hand: %zu cards\n", game->player_hand.len);
      // printw("===========================\n\n");
      // printw("&game.deck.head->data: ");
    } else if (userAnswer == 'N') {
      printf("\nOK! Exit the game\n");
      cbreak();  // Disable line buffering
    } else {
      printf("\n\ninvalid answer. please try again.\n\n");
    }
  }
  return 0;
}

// 2. Betting:

int betting(WINDOW *win, BlackJackGameState *game) {
  if (game->cash < 10) {
    printf("You don't have enough cash...\nGOODBYE!");
    game_screen_end();
  } else {
    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 4, 10, "=== Betting Phase ===");
    mvwprintw(win, 8, 2, "Cash:         $%d", game->cash);
    mvwprintw(win, 9, 2, "Pot:          $%d", game->pot);
    mvwprintw(win, 10, 2, "Deck size:    %zu cards", game->deck.len);
    mvwprintw(win, 11, 2, "Dealer hand:  %zu cards", game->dealer_hand.len);
    mvwprintw(win, 12, 2, "Player hand:  %zu cards", game->player_hand.len);
    mvwprintw(win, 16, 10, "\n===========================\n");
    wrefresh(win);  // Refresh the window to show changes
    printw("You have $%d in cash.\n", game->cash);
    printw("Please enter your bet amount (minimum $10): ");
    printw("\nNote: You can bet up to your current cash amount.\n");
    printw("Enter your bet amount: ");
    int bet_amount = 0;
    int ch;
    while (true) {
      ch = getch();
      if (ch == '\n' || ch == '\r') {
        if (bet_amount >= 10 && bet_amount <= game->cash) {
          game->pot += bet_amount;
          game->cash -= bet_amount;
          printw("\nBet accepted! You bet $%d.\n", bet_amount);
          break;
        } else {
          printw("\nInvalid bet amount. Please enter a valid amount: ");
          bet_amount = 0;  // Reset bet amount
        }
      } else if (ch >= '0' && ch <= '9') {
        bet_amount = bet_amount * 10 + (ch - '0');
        printw("%c", ch);     // Echo the digit
      } else if (ch == 27) {  // Escape key to exit betting
        printw("\nBetting cancelled. Exiting...\n");
        return -1;  // Exit betting phase
      }

      else {
        printw(
            "\nInvalid input. Please enter a digit (0-9) or press Enter to "
            "confirm: ");
      }
      refresh();  // Refresh the screen to show changes
    }
    printw("\nYour current cash is $%d and the pot is $%d.\n", game->cash,
           game->pot);
    printw("Press any key to continue...\n");
    getch();  // Wait for user input to continue
  }
  return 0;  // Return success
}

int initial_deal(WINDOW *win, BlackJackGameState *game) {
  if (!game) return -1;
  game_screen_print("Dealing cards...\n");
}


// int screen() {
//   initscr();             // Initialize ncurses
//   noecho();              // Don't echo user input
//   cbreak();              // Don't buffer input (read char by char)
//   keypad(stdscr, TRUE);  // Enable special keys (like arrow keys)

//   printw("Welcome to Blackjack!\n");
//   printw("Press 'q' to quit at any time.\n");
//   refresh();

//   // --- Simulate a game turn ---
//   int choice;
//   do {
//     // Main game display (simplified)
//     mvprintw(5, 5, "Your hand: Ace, 7 (18)");
//     mvprintw(6, 5, "Dealer's hand: King, ?");
//     refresh();

//     // --- Dialog for player action ---
//     WINDOW *dialog_win =
//         newwin(5, 40, 10, 10);  // 5 rows, 40 cols, at row 10, col 10
//     box(dialog_win, 0, 0);      // Draw a border
//     mvwprintw(dialog_win, 1, 2, "What do you want to do?");
//     mvwprintw(dialog_win, 2, 2, "[H]it  [S]tand");
//     wrefresh(dialog_win);

//     choice = wgetch(dialog_win);  // Get input specific to this window

//     delwin(dialog_win);  // Delete the dialog window

//     // Process choice (simplified)
//     if (choice == 'h' || choice == 'H') {
//       mvprintw(8, 5, "You chose to Hit!");
//     } else if (choice == 's' || choice == 'S') {
//       mvprintw(8, 5, "You chose to Stand!");
//     } else {
//       mvprintw(8, 5, "Invalid choice. Try again.");
//     }
//     clrtoeol();  // Clear to end of line
//     refresh();

//   } while (choice != 'q' && choice != 'Q');  // Exit loop if 'q' is pressed

//   mvprintw(LINES - 1, 0, "Thanks for playing! Press any key to exit.");
//   getch();   // Wait for a key press before exiting
//   endwin();  // End ncurses mode
//   return 0;
// }

int main(int argc, char const *argv[]) {
  init_ui();

  WINDOW *win = newwin(100, 100, 0, 0);
  BlackJackGameState *game;

  keypad(win, TRUE);
  // screen();

  int done = 0;

  while (!done) {
    display_game(win);
    char action = get_user_action(win);
    if (action == 'N' || action == 'N') {
      shutdown_ui();
      return 0;
    } else if (action == 'y' || action == 'Y') {
      game_state_init(game);
      game_screen_main(win, game);
      betting(win, game);
      initial_deal(win, game);
      // game_screen_main(win, game);
    }
    return 0;
  }
}

// if:
// is_user_can_bet:
// check bet amount;
// move from cash to pot;
// print values;
// else:
// print error;
// exit

// 3. Initial Deal:
//  move 2 cards from deck to player.
// show 2 cards
// if blackjeck:
// user win
// calc prize = pot * 1.5
// move prize from pot to cash;
// reset:
//  pot
// cards
// start again

// else:
//  move 2 cards from deck to dealer.
//  show 1 cards

// 4. Hit or Stand
// loop:
// if hit:
// move card from deck to user list
// if stand:
// dealer turn:
// calc dealer hand
// if dealer hand > 17:
// stand
// else:
// hit
// if dealer hand > 21:
// dealer lose
// else if dealer hand == user hand:
// draw
// else dealer hand > user hand && dealer hand <=21:
// dealer win

// 5. Dealer draw

// 6. Reset Cards.