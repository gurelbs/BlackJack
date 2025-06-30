
--------------------------------------------------------------------------------
Blackjack Game - Embedded C Project
This project implements a terminal-based Blackjack game in C, where a human player competes against a computer-controlled dealer. The development emphasizes principles crucial for Embedded C systems, such as efficient memory management, bitwise operations for data encoding, and robust input handling, while adhering to strict architectural constraints.
Project Overview
The game simulates a standard Blackjack experience with a defined flow of phases: Game Initialization, Betting, Initial Deal, Blackjack Check, Player's Turn (Hit or Stand), Dealer's Turn, and Card Reset for a new round. The player manages their "cash" and a "pot" for betting. All interactions occur via the terminal, displaying the current game state, cards in hand, and requiring user input for actions like placing bets or drawing additional cards.
Core Technical Requirements & Constraints
This project is built with a strong focus on Embedded C principles, meaning certain design choices are mandatory and reflect critical engineering principles for real-time and embedded systems:
•
No Global Variables (except for constants): The game state (deck, player's hand, dealer's hand, player's cash, and the pot) must not be stored in global variables or arrays. Instead, a central GameState struct (or similar) should be used. A pointer to this struct must be passed as an argument to any function that needs to access or modify the game state. This approach promotes modularity, testability, and prevents unintended side effects, and is crucial for multi-threading environments (though this project is single-threaded).
•
Card Representation with Bitwise Operations: Each playing card is represented by a Card struct, which contains a uint8_t data field and a Card *next pointer for linked list functionality.
◦
The suit is encoded in the first 4 bits [3:0] of the data field. Each suit is represented by a single bit being set to 'ON' (e.g., Hearts: 0001, Clubs: 0010, Diamond: 0100, Spades: 1000).
◦
The rank is encoded in the next 4 bits [7:4] of the data field (e.g., Ace: 0001, 10: 1010, Jack: 1011, Queen: 1100, King: 1101).
◦
Crucially, direct bitwise operations (AND (&), OR (|), Shift (<<, >>)) must be used for reading and writing rank and suit values. The use of Bit Fields and Unions is strictly forbidden for this purpose. This ensures precise bit control and portability across different compilers and architectures, which is a fundamental skill in embedded development. Bitwise operations are generally fast and directly supported by the CPU.
•
Dynamic Memory Allocation & Linked Lists: The game's deck and player/dealer hands are represented as singly linked lists of Card structs, managed by a CardList struct (which includes a head pointer to the first card and a len field indicating the list's size).
◦
Each card must be individually dynamically allocated using malloc.
◦
A critical strategy for efficiency and to prevent memory fragmentation, particularly in memory-constrained embedded systems, is the "Object Pool" approach: when moving cards between the deck and hands (e.g., drawing a card or returning it), existing Card nodes are detached from one list and pushed onto another by simply changing pointers, without re-allocating or freeing memory during gameplay. All memory allocated for cards must be freed using free only once, before the program terminates.
•
Robust Input Validation: While the type of input is assumed to be valid (e.g., an integer when scanning for an int), the value of the input must be thoroughly validated. For example, a bet amount must be positive, a multiple of 10, and not exceed the player's available cash. The program must continuously prompt the user for input, provide clear warnings, and re-ask until a valid value is entered.
•
Random Number Generation: srand(time(NULL)) must be called only once at the very beginning of the program to initialize the pseudo-random number generator. Subsequently, rand() is used to produce pseudo-random numbers, and the modulo operator (%) is used for range limitation (e.g., rand() % 52 to get an index between 0 and 51).
•
Ace Value Calculation: The hand value calculation must correctly handle Aces, which can be valued as 1 or 11. The logic should ensure that an Ace is counted as 11 if doing so does not cause the hand total to exceed 21; otherwise, it is counted as 1.
•
Dealer AI Logic: The dealer has specific rules for drawing cards: they will keep drawing until their hand value is greater than the player's hand value or reaches 17 or more (even if this means the dealer loses). The dealer does not stop at ties, unlike the player, and the dealer does not get an auto-win for Blackjack.
Game Phases
The game proceeds through a series of defined phases with specific transitions between them:
1.
Game Initialization: Performed once at the program's start. It involves creating and populating a 52-card deck, initializing empty player and dealer hands, and setting the player's initial cash to 1000 and pot to 0.
2.
Betting: Displays current cash and pot. The player is asked to bet an amount, which is deducted from cash and added to the pot. Input validation ensures the bet is valid. The game ends if the player runs out of cash to bet.
3.
Initial Deal: Two cards are drawn for the dealer and two for the player from the deck. The cards are displayed, but the dealer's second card remains hidden.
4.
Blackjack Check: Calculates the player's hand value. If it's 21 (Blackjack), the player wins (1.5x the pot), the pot resets, and the game proceeds to the "Reset Cards" phase.
5.
Hit or Stand: The player can draw additional cards ("hit") as long as their hand value does not exceed 21. If the player chooses "stand" or "busts" (hand > 21), the game moves to the next phase.
6.
Dealer Draw: The dealer draws cards according to their rules (until hand > player's hand or >= 17). Outcomes (dealer bust, dealer win, tie) are determined, and cash/pot are updated accordingly.
7.
Reset Cards: All cards from both hands are returned to the deck. The system checks if the player has enough cash to continue. The player is asked if they wish to play another round.
Development Strategy & Tips
•
Modular Design (Bottom-Up): It is highly recommended to break down the project into smaller, dedicated functions. Start by implementing core functionalities like card representation, bitwise operations, and linked list manipulations, then build up the higher-level game logic.
•
State Machine Approach: The project's phase-based flow lends itself very well to a Finite State Machine (FSM) implementation. An enum can represent the game states (phases), and a main game loop could use a switch-case structure to call functions that handle each specific phase.
•
Helper Functions: Implementing dedicated helper functions for common tasks will simplify the code and improve readability. Examples include:
◦
void init_cardlist(CardList *cardlist)
◦
void fill_deck(CardList *cardlist)
◦
void clear_cardlist(CardList *cardlist)
◦
int get_rank_value(Card *card)
◦
int get_suit_value(Card *card)
◦
Card *card_new(int rank, int suit)
◦
Card *card_remove_at(CardList *cardlist, size_t index)
◦
Card *card_draw(CardList *cardlist)
◦
void card_push(CardList *cardlist, Card *card)
◦
int calculate_hand_value(CardList *hand)
◦
void print_hand(CardList *hand, bool hide_second_card)
◦
Functions for user input and validation (e.g., get_user_bet(), get_user_choice_hit_stand()).
•
NULL Pointer Handling: Although malloc is assumed not to return NULL for this project, correctly handling NULL pointers (e.g., checking if head is NULL when traversing a linked list) is crucial for robust C programming and preventing Segmentation Faults.
•
Memory Management Best Practices: Ensure all malloc'd memory is free'd correctly before program termination. This typically involves calling clear_cardlist on the main deck list at the end of the game's execution.
TODO List - Development Checklist
This list outlines the key tasks for developing the Blackjack game, serving as a progress tracker.
Phase 1: Core Data Structures & Bitwise Operations
•
[ ] Define Card struct: uint8_t data; and struct Card *next;.
•
[ ] Define CardList struct: struct Card *head; and size_t len;.
•
[ ] Implement Card *card_new(int rank, int suit):
◦
[ ] Dynamically allocate a Card node using malloc.
◦
[ ] Encode rank into data using bitwise shift (<< 4) and OR (|).
◦
[ ] Encode suit into data using bitwise OR (|).
◦
[ ] Set next to NULL.
◦
[ ] Return the newly created Card pointer.
•
[ ] Implement int get_rank_value(const Card *card): Extract rank from card->data using bitwise shift (>> 4).
•
[ ] Implement int get_suit_value(const Card *card): Extract suit from card->data using bitwise AND (& 0x0F).
•
[ ] Implement const char *get_rank_name(int rank_value): Map rank integer to its string representation (e.g., "Ace", "King"). (Consider using a global constant char array for names).
•
[ ] Implement const char *get_suit_name(int suit_value): Map suit integer to its string representation (e.g., "Hearts", "Clubs"). (Consider using a global constant char array for names).
•
[ ] Implement void print_card(const Card *card, bool hide_card): Print card rank and suit name (or "?????????" if hide_card is true).
Phase 2: Linked List Management & Object Pool Strategy
•
[ ] Implement void init_cardlist(CardList *cardlist): Set head to NULL and len to 0.
•
[ ] Implement void card_push(CardList *cardlist, Card *card):
◦
[ ] Add the card node to the end of the cardlist.
◦
[ ] Increment cardlist->len.
•
[ ] Implement Card *card_remove_at(CardList *cardlist, size_t index):
◦
[ ] Remove the card node at the specified index from cardlist.
◦
[ ] Decrement cardlist->len.
◦
[ ] Handle edge cases: removing from an empty list, removing the head, removing the only element, invalid index.
◦
[ ] Return the pointer to the removed Card node (this function does NOT free the memory).
•
[ ] Implement void clear_cardlist(CardList *cardlist):
◦
[ ] Iterate through cardlist, free each Card node.
◦
[ ] Reset head to NULL and len to 0.
Phase 3: Game Flow Implementation
•
[ ] Define GameState struct: CardList deck;, CardList player_hand;, CardList dealer_hand;, int cash;, int pot;. (Note: these should be CardList objects, not pointers, if the struct is passed by pointer to functions. If passed by value, they would be copied, which is not ideal for large structures, but sources recommend pointer for GameState). Let's stick with the recommended CardList * for hands and deck within GameState if GameState itself is passed as a pointer. Let's adjust for clarity.
◦
typedef struct { CardList deck; CardList player_hand; CardList dealer_hand; int cash; int pot; } GameState;
•
[ ] Game Initialization Phase (void game_init(GameState *state)):
◦
[ ] Call init_cardlist for state->deck, state->player_hand, state->dealer_hand.
◦
[ ] Call fill_deck(&(state->deck)): Populate the deck with 52 standard cards.
◦
[ ] Set state->cash to 1000, state->pot to 0.
◦
[ ] Call srand(time(NULL)) only once.
•
[ ] Betting Phase (void game_betting(GameState *state)):
◦
[ ] Display state->cash and state->pot.
◦
[ ] Implement game over check: if state->pot == 0 and state->cash < 10, notify player and exit program.
◦
[ ] Implement input loop for bet amount with validation: positive, multiple of 10, not exceeding cash, allowing 0 only if pot > 0.
◦
[ ] Deduct bet from state->cash, add to state->pot.
•
[ ] Initial Deal Phase (void game_initial_deal(GameState *state)):
◦
[ ] Implement Card *card_draw(CardList *cardlist): Draws a random card using rand() % cardlist->len and card_remove_at.
◦
[ ] Draw 2 cards for dealer and 2 for player from state->deck.
◦
[ ] Push drawn cards to respective hands using card_push.
◦
[ ] Display hands, hiding the dealer's second card.
•
[ ] Blackjack Check Phase (void game_blackjack_check(GameState *state)):
◦
[ ] Implement int calculate_hand_value(CardList *hand): Sum card values, correctly handle Ace (1 or 11) logic.
◦
[ ] Calculate player's hand value.
◦
[ ] If player has 21 (Blackjack), print "Black Jack!", add state->pot and 1.5 * state->pot to state->cash, reset state->pot to 0, and transition to Reset Cards phase.
•
[ ] Hit or Stand Phase (void game_hit_stand(GameState *state)):
◦
[ ] Loop for player's turn.
◦
[ ] Prompt user for "hit" or "stand" input with validation (case-insensitive input).
◦
[ ] If "hit": Draw a card from state->deck and push to state->player_hand. Re-calculate player's hand value. If bust (>21), print "bust", player loses, go to Reset Cards phase. Else, continue loop.
◦
[ ] If "stand": Exit loop and transition to Dealer Draw phase.
•
[ ] Dealer Draw Phase (void game_dealer_draw(GameState *state)):
◦
[ ] Reveal dealer's hidden card.
◦
[ ] Loop: Dealer draws cards from state->deck and pushes to state->dealer_hand. Re-calculate dealer's hand value after each draw.
◦
[ ] Loop condition: continue drawing as long as dealer's hand value is less than or equal to player's hand value, AND less than 17.
◦
[ ] After loop, determine outcome:
▪
[ ] If dealer busts (>21): Print "dealer bust", add 2 * state->pot to state->cash, reset state->pot to 0, go to Reset Cards.
▪
[ ] If dealer's hand == player's hand (and <= 21): Print "tie", state->pot remains, go to Reset Cards.
▪
[ ] If dealer's hand > player's hand (and <= 21): Print "dealer wins", reset state->pot to 0, go to Reset Cards.
•
[ ] Reset Cards Phase (void game_reset_cards(GameState *state)):
◦
[ ] Move all cards from state->player_hand and state->dealer_hand back to state->deck using card_push (object pool strategy).
◦
[ ] Check for game over condition: if state->cash < 10, print "out of cash to bet. Game Over." and set flag to terminate program.
◦
[ ] Otherwise, prompt player to play again ("yes"/"no"). If "yes", transition to Betting phase. If "no", set flag to terminate program.
Phase 4: Main Game Loop & Final Cleanup
•
[ ] Implement int main() function:
◦
[ ] Declare a GameState variable.
◦
[ ] Call game_init for initial setup.
◦
[ ] Implement a main game loop (e.g., do-while or FSM using switch-case as suggested) that transitions between game phases based on outcomes.
◦
[ ] After the main loop, call clear_cardlist(&(state.deck)) to free all dynamically allocated card memory.
◦
[ ] Return 0.
Phase 5: Testing & Debugging
•
[ ] Thoroughly test card_new, get_rank_value, get_suit_value.
•
[ ] Test card_push and card_remove_at for various scenarios (empty list, head, middle, tail, invalid index).
•
[ ] Test fill_deck and clear_cardlist for correct memory allocation and deallocation.
•
[ ] Test all input validation routines.
•
[ ] Test all game outcomes: player Blackjack, player bust, dealer bust, dealer win, player win, tie.
•
[ ] Test game restart (multiple rounds) and game over conditions (out of cash, player quits).
•
[ ] Use debugging tools (e.g., Valgrind for memory leaks if on Linux) to ensure no memory leaks or invalid memory accesses.

--------------------------------------------------------------------------------