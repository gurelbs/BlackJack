#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define CARDS_NUM 52

typedef enum Suits {
  HEART = 0x01,
  CLUBS = 0x02,
  DIAMONDS = 0x04,
  SPADES = 0x08,
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

//  Game Initialization

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

void fill_deck(CardsList *deck) {
  if (!deck) return;
  int suits[] = {HEART, CLUBS, DIAMONDS, SPADES};

  // Debug prints to see the calculation
  printf("sizeof(suits) = %zu bytes\n", sizeof(suits));                 // 16
  printf("sizeof(suits[0]) = %zu bytes\n", sizeof(suits[0]));           // 4
  printf("Number of suits = %zu\n", sizeof(suits) / sizeof(suits[0]));  // 4

  size_t suits_num = sizeof(suits) / sizeof(suits[0]);
  size_t ranks_num = CARDS_NUM / suits_num;

  for (size_t i = 0; i < suits_num; i++) {
    for (size_t rank = 1; rank <= ranks_num; rank++) {
      Card *new_card = create_card(rank, suits[i]);

      if (new_card) {
        printf("Created: %zu of %zu (data: 0x%02X)\n", rank, i, new_card->data);
        append_card(deck, new_card);
      }
    }
  }
};

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

int game_init() {
  char userAnswer;
  printf("\nWellcome to my black jack game!!\n");

  while (true) {
    printf("are you ready to play?\tclick Y to YES or N to NO:\t");
    scanf(" %c", &userAnswer);
    while (getchar() != '\n' && getchar() != EOF);
    if (userAnswer == 'Y') {
      printf("\nOK! lets start the game!\ninitializing...\n\n");
      BlackJackGameState game;
      game_state_init(&game);

      // Uncomment and replace the debug prints
      printf("=== Current Game Status ===\n");
      printf("Cash: $%d\n", game.cash);
      printf("Pot: $%d\n", game.pot);
      printf("Deck size: %zu cards\n", game.deck.len);
      printf("Dealer hand: %zu cards\n", game.dealer_hand.len);
      printf("Player hand: %zu cards\n", game.player_hand.len);
      printf("===========================\n\n");

    } else if (userAnswer == 'N') {
      printf("\nOK! Exit the game\n");
      break;
    } else {
      printf("\n\ninvalid answer. please try again.\n\n");
    }
  }
  return 0;
}

int main() { game_init(); }
