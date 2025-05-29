#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>

struct CardsList
{
    struct Card *head;
    size_t len;
};

struct Card
{
    uint8_t data;
    struct Card *next;
};

struct BlackJackGameState
{
    struct CardsList cards_list;
    struct Card card;
};

int game_init() {
    char userAnswer;
    printf("\nWellcome to my black jack game!!\n");
    
    while (true) {
        printf("are you ready to play?\tclick Y to YES or N to NO:\t");
        scanf(" %c", &userAnswer);
        while (getchar() != '\n');
        if (userAnswer == 'Y'){
            printf("\nOK! lets start the game...\n");
            printf("initializing game...\n");
            break;
        } else if (userAnswer == 'N') {
            printf("\nOK! Exit the game\n");
            break;
        } else {
            printf("\n\ninvalid answer. please try again.\n\n");
        }
    }
    return 0;    
}


int main() {
    game_init();
}
