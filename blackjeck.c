#include "blackjeck.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int cards_list_init(CardsList *list) {
  if (!list)
    return -1;
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
  new_card->data = (uint8_t)(((rank & RANK_MASK) | (suit << 4)));
  new_card->next = NULL;
  return new_card;
}

void append_card(CardsList *list, Card *card) {
  if (!list || !card)
    return;

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
  if (!deck)
    return;
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
  game->cash = CASH;
  game->pot = 0;
  game->hands_won = 0;
  printf("\n====================================\n");
  printf("\t~ BlackJack Game ~\n");
  printf("\tBy Gurel Ben Shabat\n");
  printf("====================================\n\n");
}

int betting(BlackJackGameState *game) {
  if (game->cash < 10 && game->pot == 0) {
    printf("You don't have enough cash and the pot is empty...\nGOODBYE!\n");
    cleanup(game);
    return -1;
  } else if (game->pot > 0) {
    // Pot has money from previous tie - skip betting
    printf("CASH:\t$%d\n", game->cash);
    printf("POT:\t$%d (from previous tie)\n", game->pot);
    printf("\nUsing existing pot from the tie. Let's continue!\n");
    return 0;
  } else {
    printf("CASH:\t%d\n", game->cash);
    printf("POT:\t%d\n", game->pot);
    printf("Please enter your bet amount (between $10 and $%d):\t", game->cash);
    unsigned int bet_amount = 0;
    int valid = 0;
    while (!valid) {
      if (scanf(" %u", &bet_amount) != 1) {
        printf("Invalid input.\nPlease enter a number:\t");
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
          ;
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
    printf("\n____________\n"
           "POT:\t$%d\nCASH:\t$%d"
           "\n‾‾‾‾‾‾‾‾‾‾‾‾\n",
           bet_amount, game->cash);
  }
  return 0;
}

Card *card_remove_at(CardsList *list) {
  long unsigned int random_index = rand() % list->len;
  size_t i = 0;
  Card *previous = NULL;
  Card *current = list->head;

  while (current != NULL && i < random_index) {
    previous = current;
    current = current->next;
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

int get_rank(uint8_t card_data) {
  int rank = card_data & RANK_MASK;
  return (!rank) ? -1 : (rank > 10) ? 10 : (rank < 2) ? 1 : rank;
}

void reset_cards_by_list(BlackJackGameState *game, CardsList *list) {
  while (list->len > 0) {
    Card *card_to_move = list->head;
    list->len--;
    list->head = list->head->next;
    card_to_move->next = NULL;
    append_card(&game->deck, card_to_move);
  }
  list->head = NULL;
  list->tail = NULL;
}

void reset_cards(BlackJackGameState *game) {
  reset_cards_by_list(game, &game->player_hand);
  reset_cards_by_list(game, &game->dealer_hand);
  if (game->cash < 10 && game->pot == 0) {
    printf("Your of cash to bet.\nGame Over!\nGoodbye...\n");
    exit(0);
  }
  ask_play_again(game);
}

int calc_total(CardsList *list) {
  int total = 0;
  int aces = 0;
  Card *current = list->head;
  while (current) {
    int rank = get_rank(current->data);
    if (rank == 1) {
      aces++;
      total += MAX_ACE_VALUE;
    } else {
      total += rank;
    }
    current = current->next;
  }

  while (total > BLACKJECK && aces > 0) {
    total -= (MAX_ACE_VALUE - ACE_VALUE);
    aces--;
  }

  return total;
}

void show_cards(CardsList *list, bool show_all) {
  static const char *ranks[] = {"?", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
  
  Card *current = list->head;
  int card_count = 0;
  
  // Count all cards in the list
  Card *temp = current;
  while (temp) {
    card_count++;
    temp = temp->next;
  }
  
  // Print cards row by row
  for (int row = 0; row < 7; row++) {
    current = list->head;
    for (int i = 0; i < card_count; i++) {
      if (!show_all && i > 0) {
        // Show hidden card with '?' symbols
        switch (row) {
          case 0: printf("┌─────────┐ "); break;
          case 1: printf("│         │ "); break;
          case 2: case 4: printf("│         │ "); break;
          case 3: printf("│    ?    │ "); break;
          case 5: printf("│         │ "); break;
          case 6: printf("└─────────┘ "); break;
        }
      } else {
        uint8_t rank = current->data & RANK_MASK;
        uint8_t suit_bits = (current->data >> 4) & RANK_MASK;
        
        const char *suit = (suit_bits & 1) ? "♥" :
                          (suit_bits & 2) ? "♣" :
                          (suit_bits & 4) ? "♦" :
                          (suit_bits & 8) ? "♠" : "?";
        
        switch (row) {
          case 0: printf("┌─────────┐ "); break;
          case 1: printf("│%s        │ ", suit); break;
          case 2: case 4: printf("│         │ "); break;
          case 3: printf("│    %s%s   │ ", ranks[rank], rank == 10 ? "" : " "); break;
          case 5: printf("│        %s│ ", suit); break;
          case 6: printf("└─────────┘ "); break;
        }
      }
      current = current->next;
    }
    printf("\n");
  }
  
  if (show_all) {
    printf("\nTotal: %d\n", calc_total(list));
  }
}

void ask_play_again(BlackJackGameState *game) {
  char answer;
  char extra_char;
  printf("Want to play again? (Y/N):\t");
  while (true) {
    if (scanf(" %c", &answer) == 1) {
      // Check if there are extra characters after the first one
      if (scanf("%c", &extra_char) == 1 && extra_char != '\n') {
        // Clear the rest of the input buffer
        while (getchar() != '\n' && getchar() != EOF);
        printf("Please enter only one character (Y or N):\t");
        continue;
      }
      
      if (answer == 'Y' || answer == 'y') {
        printf("Alright, let's go again!\n");
        game_loop(game);
      } else if (answer == 'N' || answer == 'n') {
        printf("\nCASH:\t\t%d\n", game->cash);
        printf("HANDS WON:\t\t%d\n", game->hands_won);
        printf("\n\nThanks for playing! See you next time!\n\n");
        cleanup(game);
        exit(0);
      } else {
        printf("Invalid input. Please enter Y (yes) or N (no):\t");
      }
    } else {
      // Clear input buffer on scanf failure
      while (getchar() != '\n' && getchar() != EOF);
      printf("Invalid input. Please enter Y (yes) or N (no):\t");
    }
  }
}

void dealer_draw(BlackJackGameState *game) {
  int total_player = calc_total(&game->player_hand);
  int total_dealer = calc_total(&game->dealer_hand);

  printf("\nDealer Cards:\n\n");
  show_cards(&game->dealer_hand, true);

  while (total_dealer < 17) {
    Card *new_card = card_remove_at(&game->deck);
    append_card(&game->dealer_hand, new_card);
    total_dealer = calc_total(&game->dealer_hand);
    printf("\nDealer draws a card:\n\n");
    show_cards(&game->dealer_hand, true);
  }

  if (total_dealer > BLACKJECK) {
    printf("Dealer busts! Player Wins!\n");
    game->cash += game->pot * 2;
    game->pot = 0;
    game->hands_won += 1;
  } else if (total_player > BLACKJECK) {
    printf("Player busts! Dealer Wins!\n");
    game->pot = 0;
  } else if (total_dealer > total_player) {
    printf("Dealer Wins!\n");
    game->pot = 0;
  } else if (total_player > total_dealer) {
    printf("Player Wins!\n");
    game->cash += game->pot * 2;
    game->pot = 0;
    game->hands_won += 1;
  } else {
    printf("It's a tie! Money stays in the pot for next round.\n");
  }

  reset_cards(game);
}

void hit_or_stand(BlackJackGameState *game) {
  char answer;
  char extra_char;
  int exit = 0;
  int total = calc_total(&game->player_hand);
  if (total == BLACKJECK && game->player_hand.len == 2) {
    printf("BLACKJECK! You Win!\n");
    game->cash += (game->pot * 2.5);
    game->pot = 0;
    game->hands_won += 1;
    reset_cards(game);
  }
  while (!exit) {
    printf("\nPlease enter H (Hit) or S (Stand):\t");
    if (scanf(" %c", &answer) == 1) {
      // Check if there are extra characters after the first one
      if (scanf("%c", &extra_char) == 1 && extra_char != '\n') {
        // Clear the rest of the input buffer
        while (getchar() != '\n' && getchar() != EOF);
        printf("Please enter only one character (H for Hit or S for Stand):\t");
        continue;
      }
      
      if (answer == 'H' || answer == 'h') {
        append_card(&game->player_hand, card_remove_at(&game->deck));
        printf("\nYour Cards:\n\n");
        show_cards(&game->player_hand, true);
        int total = calc_total(&game->player_hand);
        if (total < BLACKJECK) {
          exit = 0;
        } else if (total > BLACKJECK) {
          exit = 1;
          printf("Dealer wins!\n");
          game->pot = 0;
          reset_cards(game);
        } else {
          exit = 1;
          dealer_draw(game);
        }
      } else if (answer == 'S' || answer == 's') {
        exit = 1;
        dealer_draw(game);
        break;
      } else {
        printf("Invalid choice.");
      }
    } else {
      // Clear input buffer on scanf failure
      while (getchar() != '\n' && getchar() != EOF);
      printf("Invalid input.");
    }
  }
}

void deallocate_cards_list(CardsList *list) {
  Card *current = list->head;
  Card *next;

  while (current != NULL) {
    next = current->next;
    free(current);
    current = next;
  }
  list->head = NULL;
  list->tail = NULL;
  list->len = 0;
}

void cleanup(BlackJackGameState *game) {
  deallocate_cards_list(&game->deck);
  deallocate_cards_list(&game->player_hand);
  deallocate_cards_list(&game->dealer_hand);
}

void game_loop(BlackJackGameState *game) {
  betting(game);
  first_dealing(game);
  printf("\nDealer Cards:\n\n");
  show_cards(&game->dealer_hand, false);
  printf("\nYour Cards:\n\n");
  show_cards(&game->player_hand, true);
  hit_or_stand(game);
}

int main() {
  srand(time(NULL));
  BlackJackGameState game;
  initializing(&game);
  game_loop(&game);
}