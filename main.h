#include <inttypes.h>
#include <ncurses.h>
#include <stdlib.h>

#define CARDS_DECK_NUM 52
#define BLACKJECK 21
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
  unsigned int hands_won;
} BlackJackGameState;

int cards_list_init(CardsList *list);
Card *create_card(unsigned int rank, unsigned int suit);
void append_card(CardsList *list, Card *card);
void fill_deck(CardsList *deck);
void initializing(struct BlackJackGameState *game);
int betting(BlackJackGameState *game);
Card *card_remove_at(CardsList *list);
void print_all_game_status(BlackJackGameState *game);
void first_dealing(BlackJackGameState *game);
int get_rank(uint8_t card_data, bool ace_rank_is_11);
void reset_cards_lists(BlackJackGameState *game, CardsList *list);
int calc_total(CardsList *list);
void show_cards(CardsList *list, bool is_dealer_hand);
void ask_play_again(BlackJackGameState *game);
void hit_or_stand(BlackJackGameState *game);
void game_loop(BlackJackGameState *game);