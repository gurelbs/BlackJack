
#include "main.h"

#include <inttypes.h>
#include <ncurses.h>  // TODO: add to read me instruction for install: sudo apt install libncurses5-dev libncursesw5-dev
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void game_loop(BlackJackGameState *game);

int cards_list_init(CardsList *list) {
  if (!list) return -1;
  list->head = NULL;
  list->tail = NULL;
  list->len = 0;
  return 0;
}

Card *create_card(unsigned int rank, unsigned int suit) {
  Card *new_card = (Card *)malloc(sizeof(Card));
  if (!new_card) {
    perror("Failed to allocate new card");
    exit(EXIT_FAILURE);
  }
  new_card->data = (uint8_t)(((rank & 0x0F) | (suit << 4)));
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
  for (size_t i = 0, card_num = 1; i < SUITES_NUM; i++) {
    for (size_t rank = 1; rank <= RANKS_NUM; rank++, card_num++) {
      Card *new_card = create_card(rank, suits[i]);
      if (new_card) {
        append_card(deck, new_card);
      }
    }
  }
}

void initializing(struct BlackJackGameState *game) {
  cards_list_init(&game->deck);
  cards_list_init(&game->dealer_hand);
  cards_list_init(&game->player_hand);
  fill_deck(&game->deck);
  game->cash = 1000;
  game->pot = 0;
  game->hands_won = 0;
  printf("\n====================================\n");
  printf("\t~ BlackJack Game ~\n");
  printf("\tBy Gurel Ben Shabat\n");
  printf("====================================\n\n");
}

int betting(BlackJackGameState *game) {
  if (game->cash < 10) {
    printf("You don't have enough cash...\nGOODBYE!\n");
    return -1;
  } else {
    printf("You have $%d cash.\n", game->cash);
    printf("Please enter your bet amount (between $10 and $%d):\t", game->cash);
    int bet_amount = 0;
    int valid = 0;
    while (!valid) {
      if (scanf(" %d", &bet_amount) != 1) {
        printf("Invalid input.\nPlease enter a number:\t");
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        continue;
      }
      if (bet_amount >= 10 && bet_amount <= game->cash) {
        printf("\nbet accepted!");
        valid = 1;
      } else {
        printf("Invalid bet amount.\nPlease enter a valid amount:\t");
      }
    }
    game->pot += bet_amount;
    game->cash -= bet_amount;
    printf(
        "\n____________\n"
        "POT:\t$%d\nCASH:\t$%d"
        "\n‾‾‾‾‾‾‾‾‾‾‾‾\n",
        bet_amount, game->cash);
  }
  return 0;
}

Card *card_remove_at(CardsList *list) {
  int random_index = rand() % list->len;
  size_t i = 0;
  Card *previous = NULL;
  Card *current = list->head;

  while (current != NULL && i < random_index) {
    previous = current;
    current = current->next;
    // printf("current data: %hhu\n", current->data);
    list->tail->next = NULL;
    i++;
  }
  if (previous) {
    previous->next = current->next;
  } else {
    list->head = current->next;
  }
  if (current == list->tail) {
    list->tail = previous;
  }

  current->next = NULL;
  list->len--;
  return current;
}

void print_all_game_status(BlackJackGameState *game) {
  printf("\n===================\n");
  printf("game.cash:\t%d\n", game->cash);
  printf("game.pot:\t%d\n", game->pot);
  printf("game.deck.len:\t%zu\n", game->deck.len);
  printf("game.player_hand.len:\t%zu\n", game->player_hand.len);
  printf("game.dealer_hand.len:\t%zu\n", game->dealer_hand.len);
  printf("===================\n");
}

void first_dealing(BlackJackGameState *game) {
  append_card(&game->player_hand, card_remove_at(&game->deck));
  append_card(&game->player_hand, card_remove_at(&game->deck));
  append_card(&game->dealer_hand, card_remove_at(&game->deck));
  append_card(&game->dealer_hand, card_remove_at(&game->deck));
}

int get_rank(uint8_t card_data, bool ace_rank_is_11) {
  int rank = card_data & RANK_MASK;
  if (!rank) return -1;
  if (rank > 10) return 10;
  if (rank < 2 && ace_rank_is_11) return 11;
  return rank;
}

int dealer_draw() {}

int reset_cards_lists(BlackJackGameState *game, CardsList *list) {
  while (list->len > 0) {
    append_card(&game->deck, list->head);
    list->head = list->head->next;
    list->len--;
  }
}

int calc_total(CardsList *list) {
  int total = 0;
  Card *current = list->head;
  while (current) {
    total += get_rank(current->data, total > BLACKJECK ? false : true);
    current = current->next;
  }
  // return total == BLACKJECK && list.len == 2 ? true : false;
  return total;
}

void show_cards(CardsList *list, bool is_dealer_hand) {
  static const char *ranks[] = {"?", "A", "2", "3",  "4", "5", "6",
                                "7", "8", "9", "10", "J", "Q", "K"};
  static const char *suits[] = {"?", "♥", "♠", "♦", "♣"};
  Card *current = list->head;
  printf("\n%s Cards:\n\n", !is_dealer_hand ? "Your" : "Dealer");
  while (current) {
    uint8_t rank = current->data & RANK_MASK;
    uint8_t suit = (current->data >> 4) & RANK_MASK;

    printf("%s%s|\n|\t|\n|   %s   |\n|\t|\n|%s%s\n", ranks[rank],
           (rank == 10) ? "‾‾‾‾‾‾" : "‾‾‾‾‾‾‾", suits[suit],
           (rank == 10) ? "______" : "_______", ranks[rank]);
    if (is_dealer_hand) {
      printf("?‾‾‾‾‾‾‾|\n|\t|\n|   ?   |\n|\t|\n|_______?\n");
      break;
    }
    current = current->next;
  }
  if (!is_dealer_hand) {
    int total = calc_total(list);
    printf("\ntotal:\t%d\n", total);
  }
}

void ask_play_again(BlackJackGameState *game) {
  char answer;
  printf("Feeling good? Let's play again! (Y/N):\t");
  while (true) {
    if (scanf(" %c", &answer) == 1) {
      if (answer == 'Y' || answer == 'y') {
        printf("Alright, let's go again!\n");
        game_loop(game);
      } else if (answer == 'N' || answer == 'n') {
        printf("CASH WON:\t\t%d\n", game->cash);
        printf("HANDS WON:\t\t%d\n", game->hands_won);
        printf("\n\nThanks for playing! See you next time!\n\n");
        exit(0);
      } else {
        printf("Just type Y or N: ");
      }
    } else {
      while (getchar() != '\n' && getchar() != EOF) {
        printf("Just type Y or N: ");
      }
    }
  }
}

int hit_or_stand(BlackJackGameState *game) {
  char answer;
  int total = calc_total(&game->player_hand);
  if (total == BLACKJECK && game->dealer_hand.len == 2) {
    printf("BLACKJECK! You Win!\n");
    game->cash += (game->pot * 2.5);
    game->pot = 0;
    game->hands_won += 1;
    print_all_game_status(game);
    ask_play_again(game);
  }
  printf("Please Enter Hit or Stand? (H/S):\t");
  while (true) {
    if (scanf(" %c", &answer) == 1) {
      if (answer == 'H' || answer == 'h') {
        append_card(&game->player_hand, card_remove_at(&game->deck));
        show_cards(&game->player_hand, false);
        int total = calc_total(&game->player_hand);
        if (total < BLACKJECK) {
          hit_or_stand(game);
        } else if (total > BLACKJECK) {
          printf("Dealer wins!\n");
          game->pot = 0;
          reset_cards_lists(game, &game->player_hand);
          reset_cards_lists(game, &game->dealer_hand);
          if (game->cash < 10) {
            printf("You Have less then $10\nGame Over!\nGoodbye...\n");
            exit(0);
          } else {
            ask_play_again(game);
          }
        } else {
          printf("moving to dealer turn...\n");
          break;
        }
        break;
      } else if (answer == 'S' || answer == 's') {
        printf("moving to dealer turn...\n");
        break;
      }
    } else {
      while (getchar() != '\n' && getchar() != EOF) {
        printf("Just type Y or N: ");
      }
    }
  }
}

void game_loop(BlackJackGameState *game) {
  betting(game);
  first_dealing(game);
  show_cards(&game->dealer_hand, true);
  show_cards(&game->player_hand, false);
  hit_or_stand(game);
};

int main() {
  srand(time(NULL));
  BlackJackGameState game;
  initializing(&game);
  game_loop(&game);
}