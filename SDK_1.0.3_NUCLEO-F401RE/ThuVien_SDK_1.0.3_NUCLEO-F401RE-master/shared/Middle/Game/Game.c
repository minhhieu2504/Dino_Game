#include "Game.h"

/* ===== Internal functions ===== */
static uint8_t Game_CheckCollision(Game* game);

void Game_Init(Game* game)
{
    game->state = MENU;
    game->score = 0;
    game->highScore = 0;
    game->currentLevel = 1;
    game->selectedStartLevel = 1;

    Dino_Reset(&game->dino);
    ObstacleManager_Reset(&game->obstacleManager);
}

void Game_Reset(Game* game)
{
    game->state = PLAYING;
    game->score = 0;
    game->currentLevel = game->selectedStartLevel;

    Dino_Reset(&game->dino);
    ObstacleManager_Reset(&game->obstacleManager);
}

void Game_UpdatePlaying(Game* game, const GameInput* input)
{
	DinoInput dinoInput;
	dinoInput.jump = input->enter;  // ENTER = nhảy

	Dino_Update(&game->dino, &dinoInput);
    ObstacleManager_Update(&game->obstacleManager, game->currentLevel);

    game->score++;

    if (Game_CheckCollision(game))
    {
        game->state = GAME_OVER;
        if (game->score > game->highScore) game->highScore = game->score;
    }
}

static uint8_t Game_CheckCollision(Game* game)
{
    return ObstacleManager_CheckCollision(
        &game->obstacleManager,
        &game->dino
    );
}

GameState Game_GetState(const Game* game) { return game->state; }
int Game_GetScore(const Game* game) { return game->score; }
int Game_GetHighScore(const Game* game) { return game->highScore; }
Dino* Game_GetDino(Game* game) { return &game->dino; }
ObstacleManager* Game_GetObstacleManager(Game* game) { return &game->obstacleManager; }
