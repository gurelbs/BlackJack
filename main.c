
#include "main.h"

#include <inttypes.h>
#include <ncurses.h>  // TODO: add to read me instruction for install: sudo apt install libncurses5-dev libncursesw5-dev
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
  if (card_ptr->data & HEART){
    printf(" | HEART");
  } else if (card_ptr->data & CLUBS){
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
  // printf("sizeof(suits) = d bytes\n", sizeof(suits));                 // 16
  // printf("sizeof(suits[0]) = %zu bytes\n", sizeof(suits[0]));           // 4
  // printf("Number of suits = %zu\n", sizeof(suits) / sizeof(suits[0]));  // 4

  size_t suits_num = sizeof(suits) / sizeof(suits[0]);
  size_t ranks_num = CARDS_NUM / suits_num;

  for (size_t i = 0, card_num = 1; i < suits_num; i++) {
    for (size_t rank = 1; rank <= ranks_num; rank++, card_num++) {
      Card *new_card = create_card(rank, suits[i]);
      if (new_card) {
        append_card(deck, new_card);
        printf("%2zu)\tBinary: ", card_num);
        print_binary_card_data(new_card->data);
        print_text_card_data(new_card);
        printf(" | hex: 0x%02X", new_card->data);
        puts("");
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

int main(void) {
  char userAnswer;
  printf("\nWellcome to my black jack game!!\n");

  while (true) {
    printf("\nare you ready to play?\tclick Y to YES or N to NO:\t");
    scanf(" %c", &userAnswer);
    while (getchar() != '\n' && getchar() != EOF);
    if (userAnswer == 'Y') {
      printf("\nOK! lets start the game!\ninitializing...\n\n");
      BlackJackGameState game;
      game_state_init(&game);
      printf("Game initialized successfully!\n");
      // Uncomment and replace the debug prints
      printf("\n\n");
      printf("=== Current Game Status ===\n");
      printf("Cash: $%d\n", game.cash);
      printf("Pot: $%d\n", game.pot);
      printf("Deck size: %zu cards\n", game.deck.len);
      printf("Dealer hand: %zu cards\n", game.dealer_hand.len);
      printf("Player hand: %zu cards\n", game.player_hand.len);
      printf("===========================\n\n");

      printf("&game.deck.head->data: ");
    } else if (userAnswer == 'N') {
      printf("\nOK! Exit the game\n");
      break;
    } else {
      printf("\n\ninvalid answer. please try again.\n\n");
    }
  }
  return 0;
}
