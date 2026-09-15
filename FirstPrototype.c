// 1ST PROTOTYPE 
#include <stdio.h>
#include <stdlib.h>

int main() {
    srand(67);

    int difficulty;
    int maxNumber;
    int i;

    for (i = 0; i < 40; i++) {
        printf("=");
    }
    printf("\n");

    printf("       BOMB DEFUSAL SIMULATOR\n");
    printf("       created by: C-Four\n");

    for (i = 0; i < 40; i++) {
        printf("=");
    }
    printf("\n");

    printf("\nINSTRUCTIONS:\n");
    printf("- A secret code has been set to arm the bomb.\n");
    printf("- Guess the correct code to defuse it.\n");
    printf("- You only have 5 TRIES before the bomb detonates!\n");
    printf("- Good luck.\n");

    for (i = 0; i < 40; i++) {
        printf("-");
    }
    printf("\n");

    printf("SELECT DIFFICULTY:\n");
    printf("1. Easy (1-10)\n");
    printf("2. Intermediate (1-50)\n");
    printf("3. Hard (1-100)\n");
    printf("4. Insane (1-500)\n");

    for (i = 0; i < 40; i++) {
        printf("-");
    }
    printf("\n");

    printf("Choose difficulty: ");
    scanf("%d", &difficulty);

    if (difficulty == 1) {
        maxNumber = 10;
    } else if (difficulty == 2) {
        maxNumber = 50;
    } else if (difficulty == 3) {
        maxNumber = 100;
    } else {
        maxNumber = 500;
    }

    int correctCode = 1 + rand() % maxNumber;
    int tries = 5;
    int bombDefused = 0;
    int guess;

    printf("\nYou have 5 tries to guess the code between 1-%d before the bomb detonates!\n", maxNumber);

    while (tries > 0) {
        printf("\nTries left: %d\n", tries);
        printf("Enter code: ");
        scanf("%d", &guess);

        if (guess == correctCode) {
            printf("Correct code! Bomb defused.\n");
            bombDefused = 1;
            break;
        } else if (guess < correctCode) {
            printf("Too low!\n");
            tries--;
        } else {
            printf("Too high!\n");
            tries--;
        }
    }

    if (!bombDefused) {
        printf("\nBOOM! The bomb exploded. The code was %d.\n", correctCode);
    }

    return 0;
}
