#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
    #include <windows.h>
#endif

#define EASY 1
#define INTERMEDIATE 2
#define HARD 3
#define INSANE 4

void clearScreen() {
    #ifdef _WIN32
        system("cls");   // Windows
    #else
        system("clear"); // Linux, macOS, and Unix
    #endif
}

void delay(int ms) {
    if (ms <= 0) return;

    #ifdef _WIN32
        Sleep((DWORD)ms); // Windows: milliseconds
    #else
        struct timespec delayTime;

        delayTime.tv_sec = ms / 1000;
        delayTime.tv_nsec = (ms % 1000) * 1000000L;

        nanosleep(&delayTime, NULL); // Linux/macOS: nanoseconds
    #endif
}

int generateRandomNum(int min, int max) {
    return rand() % (max - min + 1) + min; // Min and Max is Inclusive
}

int main() {
    srand(time(NULL));

    bool isExit = false;

    while(!isExit) {
        clearScreen();
    
        printf("-- INSTRUCTIONS --------------------------------------\n");
        printf(" A secret defusal code has been set to arm the bomb.\n");
        printf(" Guess the correct defusal code to defuse it.\n");
        printf(" You only have 5 TRIES before the bomb detonates!\n");
        printf("------------------------------------------------------\n");
        printf("\n");

        int difficulty = 0;
        printf("----- SELECT DIFFICULTY -----\n");
        printf("[1] - Easy (1-10)\n");
        printf("[2] - Intermediate (1-50)\n");
        printf("[3] - Hard (1-100)\n");
        printf("[4] - Insane (1-500)\n");
        printf("-----------------------------\n");
        printf("Enter: ");
        scanf("%d", &difficulty);

        // Input validation
        if (difficulty < 1 || difficulty > 4) {
            printf("Invalid Input: Expected value from 1 to 4");
            delay(2000); // Wait two seconds before clearing the screen
            continue;
        }

        int defusalCode = 0;
        int maxNum = 0;
        switch(difficulty) {
            case EASY:
                maxNum = 10;
                defusalCode = generateRandomNum(1, maxNum);
                break;
            case INTERMEDIATE:
                maxNum = 50;
                defusalCode = generateRandomNum(1, maxNum);
                break;
            case HARD:
                maxNum = 100;
                defusalCode = generateRandomNum(1, maxNum);
                break;
            case INSANE:
                maxNum = 500;
                defusalCode = generateRandomNum(1, maxNum);
                break;
        }

        int tries = 5;
        bool isDefused = false;

        while (tries > 0) {
            clearScreen();

            printf("You have 5 tries to guess the defusal code between 1-%d before the bomb detonates!\n", maxNum);
            printf("\n");

            printf("----------------\n");
            printf("Tries left: %d\n", tries);
            printf("----------------\n");
            printf("\n");

            int guess = 0;
            printf("Enter Code: ");
            scanf("%d", &guess);
            printf("\n");

            if (guess < defusalCode) {
                printf("\n- Too low!");
            } else if (guess > defusalCode) {
                printf("\n- Too High!");
            } else {
                isDefused = true;
                break; 
            }
            
            if (tries > 1) {
                delay(2000);
            }
            tries--;
        }
        clearScreen();

        if (isDefused) {
            printf("-----------------------------\n");
            printf("Correct code! Bomb defused.\n");
            printf("-----------------------------\n");
        } else {
            printf("---------------------------------------------------------\n");
            printf("BOOM! The bomb exploded. The defusal code was %d.\n", defusalCode);   
            printf("---------------------------------------------------------\n");
        }

        printf("\n");

        char again;
        printf("--- Would you like to try again? ---\n");
        printf("[Y] - Yes\n");
        printf("[N] - No\n");
        printf("------------------------------------\n");
        printf("Enter: ");
        scanf(" %c", &again);

        // Any choice except Y Is NO
        if (again != 'Y' && again != 'y') {
            isExit = true;
        }
    }

    return 0;
}
