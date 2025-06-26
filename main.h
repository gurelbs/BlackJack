#include <inttypes.h>
#include <ncurses.h>
#include <stdlib.h>

#define CARDS_DECK_NUM 52
#define SUITES_NUM 4
#define RANKS_NUM (CARDS_DECK_NUM / SUITES_NUM)
#define RANK_MASK 0x0F

typedef enum Suits {
  HEART = 1,
  CLUBS,
  DIAMONDS,
  SPADES,
} Suits;

typedef struct Card {
  uint8_t data;
  struct Card *next;
} Card;

typedef struct CardsList {
  Card *head;
  Card *tail;
  size_t len;
} CardsList;

typedef struct BlackJackGameState {
  struct CardsList deck;
  struct CardsList dealer_hand;
  struct CardsList player_hand;
  unsigned int cash;
  unsigned int pot;
} BlackJackGameState;

int cards_list_init(CardsList *list);
Card *create_card(unsigned int rank, unsigned int suit);
void append_card(CardsList *list, Card *card);
void fill_deck(CardsList *deck);
int game_state_init(struct BlackJackGameState *game);
void game_screen_init(void);
void game_screen_end(void);
void game_screen_main(WINDOW *win, const BlackJackGameState *game);
int game_screen_print(const char *message);
