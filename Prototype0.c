#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 700
#define GAME_TIME 90.0f
#define MAX_TRIES 5

typedef enum {
    MENU,
    DIFFICULTY,
    PLAYING,
    PAUSED,
    WIN,
    LOSE,
    HOW_TO_PLAY
} GameState;

typedef enum {
    EASY = 1,
    INTERMEDIATE,
    HARD,
    INSANE
} Difficulty;

typedef struct {
    int maxNumber;
    int correctCode;
    int guess;
    int tries;
    float timeLeft;
    int inputLength;
    char input[16];
    Difficulty difficulty;
} Game;

void ResetGame(Game *game) {
    game->guess = 0;
    game->tries = MAX_TRIES;
    game->timeLeft = GAME_TIME;
    game->inputLength = 0;
    game->input[0] = '\0';

    switch (game->difficulty) {
        case EASY:
            game->maxNumber = 10;
            break;

        case INTERMEDIATE:
            game->maxNumber = 50;
            break;

        case HARD:
            game->maxNumber = 100;
            break;

        case INSANE:
            game->maxNumber = 500;
            break;
    }

    game->correctCode = 1 + rand() % game->maxNumber;
}

void DrawCenteredText(const char *text, int y, int fontSize, Color color) {
    int width = MeasureText(text, fontSize);
    DrawText(text, (SCREEN_WIDTH - width) / 2, y, fontSize, color);
}

void DrawPanel(int x, int y, int width, int height) {
    DrawRectangle(x, y, width, height, (Color){25, 25, 25, 255});
    DrawRectangleLinesEx((Rectangle){x, y, width, height}, 3, GOLD);
}

void DrawButton(Rectangle button, const char *text, int fontSize, Color normal, Color hover) {
    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, button);

    Color color = hovered ? hover : normal;

    DrawRectangleRec(button, color);
    DrawRectangleLinesEx(button, 3, GOLD);

    int textWidth = MeasureText(text, fontSize);

    DrawText(
        text,
        button.x + (button.width - textWidth) / 2,
        button.y + (button.height - fontSize) / 2,
        fontSize,
        BLACK
    );
}

void DrawBomb(float pulse) {
    int centerX = SCREEN_WIDTH / 2;
    int centerY = 285;

    DrawCircle(centerX, centerY, 105 + pulse, (Color){35, 35, 35, 255});
    DrawCircleLines(centerX, centerY, 105 + pulse, GOLD);

    DrawRectangle(centerX - 55, centerY - 60, 110, 120, BLACK);
    DrawRectangleLines(centerX - 55, centerY - 60, 110, 120, GOLD);

    DrawCircle(centerX, centerY, 38, DARKGRAY);
    DrawCircleLines(centerX, centerY, 38, GOLD);

    DrawRectangle(centerX - 20, centerY - 20, 40, 40, RED);

    DrawLine(centerX + 50, centerY - 65, centerX + 90, centerY - 105, GOLD);
    DrawLine(centerX + 90, centerY - 105, centerX + 105, centerY - 105, GOLD);

    DrawCircle(centerX + 110, centerY - 105, 7 + pulse / 2, RED);
}

void DrawHeader() {
    DrawRectangle(0, 0, SCREEN_WIDTH, 90, BLACK);
    DrawRectangle(0, 87, SCREEN_WIDTH, 3, GOLD);

    DrawText("C-FOUR", 35, 25, 38, GOLD);
    DrawText("BOMB DEFUSAL SIMULATOR", 230, 31, 27, RAYWHITE);

    DrawText("C", 900, 22, 40, GOLD);
    DrawText("4", 932, 30, 30, RAYWHITE);
}

void DrawTimer(float timeLeft) {
    int total = (int)timeLeft;

    int minutes = total / 60;
    int seconds = total % 60;

    char timer[16];

    sprintf(timer, "%02d:%02d", minutes, seconds);

    Color timerColor = GOLD;

    if (timeLeft <= 15) {
        timerColor = RED;

        if (((int)(timeLeft * 4)) % 2 == 0) {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){255, 0, 0, 18});
        }
    }

    DrawText("TIME", 70, 120, 22, RAYWHITE);
    DrawText(timer, 70, 145, 52, timerColor);
}

void DrawAttempts(int tries) {
    int startX = 70;

    DrawText("ATTEMPTS", startX, 230, 22, RAYWHITE);

    for (int i = 0; i < MAX_TRIES; i++) {
        Color color = i < tries ? GOLD : DARKGRAY;

        DrawRectangle(
            startX + i * 58,
            265,
            42,
            20,
            color
        );
    }
}

void DrawInputBox(Game *game) {
    Rectangle box = {70, 350, 300, 70};

    DrawRectangleRec(box, BLACK);
    DrawRectangleLinesEx(box, 3, GOLD);

    if (game->inputLength == 0) {
        DrawText("ENTER CODE", 90, 370, 25, DARKGRAY);
    } else {
        DrawText(game->input, 90, 370, 30, GOLD);
    }
}

void DrawDifficultyName(Game *game) {
    const char *name = "";

    switch (game->difficulty) {
        case EASY:
            name = "EASY";
            break;

        case INTERMEDIATE:
            name = "INTERMEDIATE";
            break;

        case HARD:
            name = "HARD";
            break;

        case INSANE:
            name = "INSANE";
            break;
    }

    DrawText("DIFFICULTY", 70, 460, 20, RAYWHITE);
    DrawText(name, 70, 488, 28, GOLD);

    char range[40];

    sprintf(range, "CODE RANGE: 1 - %d", game->maxNumber);

    DrawText(range, 70, 530, 20, RAYWHITE);
}

void DrawMenu() {
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);

    DrawCenteredText("C-FOUR", 80, 70, GOLD);
    DrawCenteredText("BOMB DEFUSAL SIMULATOR", 160, 30, RAYWHITE);

    DrawRectangle(370, 215, 260, 4, GOLD);

    DrawButton(
        (Rectangle){350, 260, 300, 65},
        "START GAME",
        25,
        GOLD,
        YELLOW
    );

    DrawButton(
        (Rectangle){350, 345, 300, 65},
        "HOW TO PLAY",
        25,
        GOLD,
        YELLOW
    );

    DrawButton(
        (Rectangle){350, 430, 300, 65},
        "EXIT",
        25,
        GOLD,
        YELLOW
    );

    DrawCenteredText("SYSTEM READY", 570, 18, DARKGRAY);
    DrawCenteredText("C-FOUR SECURITY SYSTEM v1.0", 600, 15, DARKGRAY);
}

void DrawDifficultyScreen() {
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);

    DrawCenteredText("SELECT DIFFICULTY", 70, 45, GOLD);

    DrawButton(
        (Rectangle){350, 170, 300, 70},
        "EASY     1-10",
        25,
        GOLD,
        YELLOW
    );

    DrawButton(
        (Rectangle){350, 260, 300, 70},
        "INTERMEDIATE     1-50",
        21,
        GOLD,
        YELLOW
    );

    DrawButton(
        (Rectangle){350, 350, 300, 70},
        "HARD     1-100",
        25,
        GOLD,
        YELLOW
    );

    DrawButton(
        (Rectangle){350, 440, 300, 70},
        "INSANE     1-500",
        25,
        GOLD,
        YELLOW
    );

    DrawCenteredText("90 SECOND TIME LIMIT", 570, 20, RAYWHITE);
}

void DrawGame(Game *game, float pulse) {
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){12, 12, 12, 255});

    DrawHeader();

    DrawPanel(40, 110, 350, 500);

    DrawTimer(game->timeLeft);
    DrawAttempts(game->tries);
    DrawInputBox(game);
    DrawDifficultyName(game);

    DrawBomb(pulse);

    DrawButton(
        (Rectangle){700, 520, 110, 55},
        "PAUSE",
        20,
        GOLD,
        YELLOW
    );

    DrawButton(
        (Rectangle){825, 520, 110, 55},
        "QUIT",
        20,
        GOLD,
        YELLOW
    );

    DrawText("ENTER = SUBMIT", 700, 595, 18, DARKGRAY);
}

void DrawPauseScreen() {
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 190});

    DrawPanel(300, 190, 400, 300);

    DrawCenteredText("GAME PAUSED", 230, 40, GOLD);
    DrawCenteredText("THE TIMER IS FROZEN", 290, 20, RAYWHITE);

    DrawButton(
        (Rectangle){350, 345, 300, 60},
        "RESUME",
        24,
        GOLD,
        YELLOW
    );

    DrawButton(
        (Rectangle){350, 420, 300, 60},
        "QUIT TO MENU",
        20,
        GOLD,
        YELLOW
    );
}

void DrawWinScreen(Game *game) {
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);

    DrawCenteredText("BOMB DEFUSED", 100, 55, GOLD);
    DrawCenteredText("MISSION COMPLETE", 175, 28, RAYWHITE);

    DrawPanel(300, 250, 400, 190);

    char codeText[50];
    char timeText[50];

    sprintf(codeText, "CODE: %d", game->correctCode);

    sprintf(
        timeText,
        "TIME REMAINING: %02d:%02d",
        (int)game->timeLeft / 60,
        (int)game->timeLeft % 60
    );

    DrawCenteredText(codeText, 285, 25, GOLD);
    DrawCenteredText(timeText, 330, 23, RAYWHITE);

    DrawCenteredText("YOU SAVED THE DAY, C-FOUR!", 390, 20, GOLD);

    DrawButton(
        (Rectangle){330, 490, 160, 60},
        "RESTART",
        20,
        GOLD,
        YELLOW
    );

    DrawButton(
        (Rectangle){510, 490, 160, 60},
        "MENU",
        20,
        GOLD,
        YELLOW
    );
}

void DrawLoseScreen(Game *game) {
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);

    DrawCenteredText("BOOM!", 100, 75, RED);
    DrawCenteredText("BOMB DETONATED", 190, 30, RAYWHITE);

    DrawPanel(300, 260, 400, 150);

    char codeText[50];

    sprintf(codeText, "THE CODE WAS: %d", game->correctCode);

    DrawCenteredText(codeText, 310, 28, GOLD);
    DrawCenteredText("MISSION FAILED", 355, 23, RAYWHITE);

    DrawButton(
        (Rectangle){330, 480, 160, 60},
        "RESTART",
        20,
        GOLD,
        YELLOW
    );

    DrawButton(
        (Rectangle){510, 480, 160, 60},
        "MENU",
        20,
        GOLD,
        YELLOW
    );
}

void DrawHowToPlay() {
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);

    DrawCenteredText("HOW TO PLAY", 70, 45, GOLD);

    DrawPanel(170, 150, 660, 370);

    DrawText("1. Choose your difficulty.", 210, 190, 22, RAYWHITE);
    DrawText("2. Find the secret code.", 210, 240, 22, RAYWHITE);
    DrawText("3. You have 5 attempts.", 210, 290, 22, RAYWHITE);
    DrawText("4. You have 90 seconds.", 210, 340, 22, RAYWHITE);
    DrawText("5. TOO LOW / TOO HIGH gives you hints.", 210, 390, 22, RAYWHITE);
    DrawText("6. Press P or click PAUSE to pause.", 210, 440, 22, RAYWHITE);

    DrawButton(
        (Rectangle){400, 570, 200, 55},
        "BACK",
        22,
        GOLD,
        YELLOW
    );
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "C-Four | Bomb Defusal Simulator");
    SetTargetFPS(60);

    srand((unsigned int)time(NULL));

    Game game;

    game.difficulty = EASY;
    ResetGame(&game);

    GameState state = MENU;

    float pulse = 0.0f;
    float lastTime = 0.0f;

    while (!WindowShouldClose()) {
        float currentTime = GetTime();
        float delta = currentTime - lastTime;

        lastTime = currentTime;

        pulse = sinf(GetTime() * 4.0f) * 3.0f;

        Vector2 mouse = GetMousePosition();

        if (state == MENU) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(
                        mouse,
                        (Rectangle){350, 260, 300, 65})) {

                    state = DIFFICULTY;
                }

                if (CheckCollisionPointRec(
                        mouse,
                        (Rectangle){350, 345, 300, 65})) {

                    state = HOW_TO_PLAY;
                }

                if (CheckCollisionPointRec(
                        mouse,
                        (Rectangle){350, 430, 300, 65})) {

                    break;
                }
            }
        }

        else if (state == HOW_TO_PLAY) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(
                        mouse,
                        (Rectangle){400, 570, 200, 55})) {

                    state = MENU;
                }
            }
        }

        else if (state == DIFFICULTY) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(
                        mouse,
                        (Rectangle){350, 170, 300, 70})) {

                    game.difficulty = EASY;
                    ResetGame(&game);
                    state = PLAYING;
                }

                else if (CheckCollisionPointRec(
                        mouse,
                        (Rectangle){350, 260, 300, 70})) {

                    game.difficulty = INTERMEDIATE;
                    ResetGame(&game);
                    state = PLAYING;
                }

                else if (CheckCollisionPointRec(
                        mouse,
                        (Rectangle){350, 350, 300, 70})) {

                    game.difficulty = HARD;
                    ResetGame(&game);
                    state = PLAYING;
                }

                else if (CheckCollisionPointRec(
                        mouse,
                        (Rectangle){350, 440, 300, 70})) {

                    game.difficulty = INSANE;
                    ResetGame(&game);
                    state = PLAYING;
                }
            }
        }

        else if (state == PLAYING) {
            game.timeLeft -= delta;

            if (game.timeLeft <= 0) {
                game.timeLeft = 0;
                state = LOSE;
            }

            if (IsKeyPressed(KEY_P)) {
                state = PAUSED;
            }

            if (IsKeyPressed(KEY_Q)) {
                state = MENU;
            }

            int key = GetCharPressed();

            while (key > 0) {
                if (key >= '0' && key <= '9') {
                    if (game.inputLength < 10) {
                        game.input[game.inputLength] = (char)key;
                        game.inputLength++;
                        game.input[game.inputLength] = '\0';
                    }
                }

                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (game.inputLength > 0) {
                    game.inputLength--;
                    game.input[game.inputLength] = '\0';
                }
            }

            if (IsKeyPressed(KEY_ENTER)) {
                if (game.inputLength > 0) {
                    game.guess = atoi(game.input);

                    if (game.guess >= 1 &&
                        game.guess <= game.maxNumber) {

                        if (game.guess == game.correctCode) {
                            state = WIN;
                        } else {
                            game.tries--;

                            game.inputLength = 0;
                            game.input[0] = '\0';

                            if (game.tries <= 0) {
                                state = LOSE;
                            }
                        }
                    } else {
                        game.inputLength = 0;
                        game.input[0] = '\0';
                    }
                }
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(
                        mouse,
                        (Rectangle){700, 520, 110, 55})) {

                    state = PAUSED;
                }

                if (CheckCollisionPointRec(
                        mouse,
                        (Rectangle){825, 520, 110, 55})) {

                    state = MENU;
                }
            }
        }

        else if (state == PAUSED) {
            if (IsKeyPressed(KEY_P)) {
                state = PLAYING;
            }

            if (IsKeyPressed(KEY_Q)) {
                state = MENU;
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(
                        mouse,
                        (Rectangle){350, 345, 300, 60})) {

                    state = PLAYING;
                }

                if (CheckCollisionPointRec(
                        mouse,
                        (Rectangle){350, 420, 300, 60})) {

                    state = MENU;
                }
            }
        }

        else if (state == WIN) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(
                        mouse,
                        (Rectangle){330, 490, 160, 60})) {

                    ResetGame(&game);
                    state = PLAYING;
                }

                if (CheckCollisionPointRec(
                        mouse,
                        (Rectangle){510, 490, 160, 60})) {

                    state = MENU;
                }
            }
        }

        else if (state == LOSE) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(
                        mouse,
                        (Rectangle){330, 480, 160, 60})) {

                    ResetGame(&game);
                    state = PLAYING;
                }

                if (CheckCollisionPointRec(
                        mouse,
                        (Rectangle){510, 480, 160, 60})) {

                    state = MENU;
                }
            }
        }

        BeginDrawing();

        if (state == MENU) {
            DrawMenu();
        }

        else if (state == DIFFICULTY) {
            DrawDifficultyScreen();
        }

        else if (state == HOW_TO_PLAY) {
            DrawHowToPlay();
        }

        else if (state == PLAYING) {
            DrawGame(&game, pulse);
        }

        else if (state == PAUSED) {
            DrawGame(&game, pulse);
            DrawPauseScreen();
        }

        else if (state == WIN) {
            DrawWinScreen(&game);
        }

        else if (state == LOSE) {
            DrawLoseScreen(&game);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}