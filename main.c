#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct Card {
  uint8_t data;
  struct Card *next;
};
struct CardsList {
  struct Card *head;
  size_t len;
};
struct BlackJackGameState {
  struct CardsList deck;
  struct CardsList dealer_hand;
  struct CardsList player_hand;
  int cash;
  int pot;
};

//  Game Initialization

int cards_list_init(struct CardsList *list, int length) {
  if (!list) return -1;
  list->head = NULL;
  list->len = length;
}

int game_state_init(struct BlackJackGameState *game) {
  if (!game) return -1;

  cards_list_init(&game->deck, 52);
  cards_list_init(&game->dealer_hand, 0);
  cards_list_init(&game->player_hand, 0);

  game->cash = 1000;
  game->pot = 0;
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
      struct BlackJackGameState game;
      game_state_init(&game);
      // Print struct fields individually
      printf("Game State:\n");
      printf("  Cash: %d\n", game.cash);
      printf("  Pot: %d\n", game.pot);
      printf("  Deck size: %zu\n", game.deck.len);
      printf("  Dealer hand size: %zu\n", game.dealer_hand.len);
      printf("  Player hand size: %zu\n", game.player_hand.len);
      puts("");
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
