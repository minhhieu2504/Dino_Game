#pragma once
#include <stdint.h>
#include "Dino.h"
#include "Obstacle.h"

/* ========= Game State ========= */
typedef enum {
    MENU,
	LEVEL_MENU,
    PLAYING,
    HIGHSCORE_VIEW,
    GAME_OVER
} GameState;

/* ========= Input ========= */
typedef struct {
    uint8_t up;
    uint8_t down;
    uint8_t left;
    uint8_t right;
    uint8_t enter;
} GameInput;

/* ========= Game ========= */
typedef struct {
    Dino dino;
    ObstacleManager obstacleManager;

    GameState state;
    int score;
    int highScore;
    int currentLevel;
    int selectedStartLevel;
} Game;

/* ========= API ========= */
void Game_Init(Game* game);
void Game_Reset(Game* game);
void Game_UpdatePlaying(Game* game, const GameInput* input);

GameState Game_GetState(const Game* game);
int Game_GetScore(const Game* game);
int Game_GetHighScore(const Game* game);
Dino* Game_GetDino(Game* game);
ObstacleManager* Game_GetObstacleManager(Game* game);
