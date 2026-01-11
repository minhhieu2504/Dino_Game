/*******************************************************************************
 *
 * Copyright (c) 2020
 * Lumi, JSC.
 * All Rights Reserved
 *
 * Description: Lab2.2
 *
 * Author: Developer embedded team
 *
 * Last Changed By:  $Author: HoangNH $
 * Revision:         $Revision: 1.0 $
 * Last Changed:     $Date: 10/7/2020 $
 *
 ******************************************************************************/
#include "stm32f401re_rcc.h"
#include "stm32f401re_gpio.h"
#include "Game.h"
#include <stdint.h>
#include <Ucglib.h>
#include <timer.h>
#include <stdio.h>

//Define Logic GPIO_PIN--------------------------------------------------------
#define GPIO_PIN_SET 				    1
#define GPIO_PIN_RESET 				    0
#define GPIO_PIN_LOW 				    0
#define GPIO_PIN_HIGH 				    1

//Define GPIO_PIN--------------------------------------------------------------
#define BUZZER_GPIO_PORT				GPIOC
#define BUZZER_GPIO_PIN                 GPIO_Pin_9
#define BUZZER_PIN9                     9
#define BUZZERControl_SetClock	        RCC_AHB1Periph_GPIOC

#define BUTTON1_GPIO_PORT				GPIOB
#define BUTTON1_GPIO_PIN 				GPIO_Pin_4
#define BUTTON_B5_PIN					4
#define BUTTONControl_SetClock			RCC_AHB1Periph_GPIOB

#define BUTTON2_GPIO_PORT               GPIOB
#define BUTTON2_GPIO_PIN                GPIO_Pin_5
#define BUTTON_B1_PIN                   5
#define BUTTON2Control_SetClock         RCC_AHB1Periph_GPIOB

#define BUTTON3_GPIO_PORT               GPIOA
#define BUTTON3_GPIO_PIN                GPIO_Pin_4
#define BUTTON_B3_PIN                   4
#define BUTTON3Control_SetClock         RCC_AHB1Periph_GPIOA

#define BUTTON4_GPIO_PORT               GPIOB
#define BUTTON4_GPIO_PIN                GPIO_Pin_3
#define BUTTON_B2_PIN                   3
#define BUTTON4Control_SetClock         RCC_AHB1Periph_GPIOB

#define BUTTON5_GPIO_PORT               GPIOB
#define BUTTON5_GPIO_PIN                GPIO_Pin_0
#define BUTTON_B4_PIN                   0
#define BUTTON5Control_SetClock         RCC_AHB1Periph_GPIOB


//function declaration---------------------------------------------------------
static ucg_t ucg;
static void Buzzer_Init(void);
static void Button_Init(void);
static void AppInitCommon(void);
static void BuzzerControl_SetStatus(GPIO_TypeDef * GPIOx, uint8_t GPIO_PIN, uint8_t Status);
static uint8_t ButtonRead_Status(GPIO_TypeDef * GPIOx, uint32_t GPIO_PIN);
static GameInput ReadGameInput(void);

static uint8_t menuIndex = 1;   // 1..3
static uint8_t needRedraw = 1;
static void LCD_Init(void);
static void UpdateMenu(const GameInput* input);
static void RenderMenu(const GameInput* input, Game* game);

static uint8_t hsNeedRedraw = 1;
static void IntToStr(int value, char* buf);
static void DrawNumber(int x, int y, int value);
static void RenderHighScore(const GameInput* input, Game* game);

static uint8_t levelIndex = 1;
static uint8_t levelNeedRedraw = 1;
static void UpdateLevelMenu(const GameInput* input);
static void RenderLevelMenu(const GameInput* input, Game* game);

static void DrawDino(const Dino* dino);
static void DrawOneCactus(int baseX, int baseY);
static void DrawObstacles(const ObstacleManager* om);

static uint8_t playingNeedClear = 1;
static Rect prevDinoRect;
static uint8_t firstFrame = 1;
static GameState lastState = -1;
static Rect prevScoreNumRect;
static uint8_t firstScoreFrame = 1;
static void DrawScoreLabel(void);
static void DrawScoreNumber(int score);
static void RenderPlaying(Game* game);

static uint8_t gameOverNeedRedraw = 1;
static void RenderGameOver(const GameInput* input, Game* game);
static void Buzzer_Init(void);
static void BuzzerControl_SetStatus(GPIO_TypeDef * GPIOx,uint8_t GPIO_PIN, uint8_t Status);
static void Delay(uint32_t ms);



//-----------------------------------------------------------------------------
int main(void)
{

	AppInitCommon();

    SystemCoreClockUpdate();
	TimerInit();

	Game game;
	GameInput input;

	Game_Init(&game);
	LCD_Init();

    while (1)
    {
    	input = ReadGameInput();
    	GameState currentState = Game_GetState(&game);

    	if (currentState != lastState)
		{
    		if (currentState == PLAYING || currentState == GAME_OVER)
			{
				BuzzerControl_SetStatus(BUZZER_GPIO_PORT, BUZZER_PIN9, GPIO_PIN_SET);
				Delay(100);
				BuzzerControl_SetStatus(BUZZER_GPIO_PORT, BUZZER_PIN9, GPIO_PIN_RESET);
			}

			ucg_ClearScreen(&ucg);

			/* reset flag render PLAYING */
			if (currentState == PLAYING)
			{
				firstFrame = 1;
				firstScoreFrame = 1;
			}
			if (currentState == HIGHSCORE_VIEW)
			{
				hsNeedRedraw = 1;
			}
			if (currentState == GAME_OVER)
			{
				gameOverNeedRedraw = 1;
				firstScoreFrame = 1;
			}

			lastState = currentState;
		}

		switch (currentState)
		{
			case MENU:
				RenderMenu(&input, &game);
				break;

			case LEVEL_MENU:
				RenderLevelMenu(&input, &game);
				break;

			case PLAYING:
				Game_Update(&game, &input);
				RenderPlaying(&game);
				break;

			case GAME_OVER:
				RenderGameOver(&input, &game);
				break;

			case HIGHSCORE_VIEW:
				RenderHighScore(&input, &game);
				break;
		}

		Delay(10);
		processTimerScheduler();
    }
}


/**
 * @func   Initializes
 * @brief  Initializes All Periperal
 * @param  None
 * @retval None
 */
static void LCD_Init(void)
{
    Ucglib4WireSWSPI_begin(&ucg, UCG_FONT_MODE_SOLID);

    ucg_SetFont(&ucg, ucg_font_helvR08_tf);
    ucg_SetRotate180(&ucg);

    ucg_SetColor(&ucg, 0, 255,255,255);
    ucg_SetColor(&ucg, 1, 0,0,0);

    ucg_ClearScreen(&ucg);
}

static void UpdateMenu(const GameInput* input)
{
    if (input->down)
    {
        if (menuIndex < 3)
        {
            menuIndex++;
            needRedraw = 1;
        }
    }

    if (input->up)
    {
        if (menuIndex > 1)
        {
            menuIndex--;
            needRedraw = 1;
        }
    }
}

static void RenderMenu(const GameInput* input, Game* game)
{
    UpdateMenu(input);

    if (input->enter)
        {
            switch (menuIndex)
            {
				case 1:
					Game_Reset(game);
					playingNeedClear = 1;   // ⭐ rất quan trọng
					return;

                case 2:
                	game->state = LEVEL_MENU;
					levelNeedRedraw = 1;
                    return;

                case 3:
                    game->state = HIGHSCORE_VIEW;
                    hsNeedRedraw = 1;   // ⭐ QUAN TRỌNG
                    return;
            }
        }

    if (!needRedraw)
        return;

    ucg_ClearScreen(&ucg);
    ucg_SetFont(&ucg, ucg_font_helvR08_tf);
    ucg_DrawString(&ucg, 27, 40, 0, "DINO GAME");

    // START GAME
    if (menuIndex == 1)
        ucg_DrawString(&ucg, 25, 70, 0, "> Start Game");
    else
        ucg_DrawString(&ucg, 25, 70, 0, "  Start Game");

    // LEVEL
    if (menuIndex == 2)
        ucg_DrawString(&ucg, 25, 84, 0, "> Level");
    else
        ucg_DrawString(&ucg, 25, 84, 0, "  Level");

    // HIGHSCORE
    if (menuIndex == 3)
        ucg_DrawString(&ucg, 25, 98, 0, "> High Score");
    else
        ucg_DrawString(&ucg, 25, 98, 0, "  High Score");

    needRedraw = 0;   // ✅ rất quan trọng
}

static void IntToStr(int value, char* buf)
{
    int i = 0;

    if (value == 0)
    {
        buf[i++] = '0';
        buf[i] = '\0';
        return;
    }

    char tmp[10];
    int j = 0;

    while (value > 0)
    {
        tmp[j++] = (value % 10) + '0';
        value /= 10;
    }

    // đảo ngược
    while (j > 0)
    {
        buf[i++] = tmp[--j];
    }

    buf[i] = '\0';
}

static void DrawNumber(int x, int y, int value)
{
    char buf[8];
    IntToStr(value, buf);

    for (int i = 0; buf[i]; i++)
    {
        ucg_DrawGlyph(&ucg, x + i * 6, y, 0, buf[i]);
    }
}

static void RenderHighScore(const GameInput* input, Game* game)
{
    /* ===== ENTER → QUAY VỀ MENU ===== */
    if (input->enter)
    {
        game->state = MENU;
        hsNeedRedraw = 1;
        needRedraw = 1;   // để menu vẽ lại khi quay về
        return;
    }

    /* ===== CHỈ VẼ 1 LẦN ===== */
    if (!hsNeedRedraw)
        return;

    ucg_ClearScreen(&ucg);
    ucg_SetFont(&ucg, ucg_font_helvR08_tf);

    ucg_DrawString(&ucg, 25, 40, 0, "HIGH SCORE");

    /* ===== SCORE ===== */
    DrawNumber(50, 64, Game_GetHighScore(game));

    hsNeedRedraw = 0;
}

static void UpdateLevelMenu(const GameInput* input)
{
	if (input->down)
	    {
	        if (levelIndex < 3)
	        {
	            levelIndex++;
	            levelNeedRedraw = 1;
	        }
	    }

	    if (input->up)
	    {
	        if (levelIndex > 1)
	        {
	            levelIndex--;
	            levelNeedRedraw = 1;
	        }
	    }
}

static void RenderLevelMenu(const GameInput* input, Game* game)
{
	UpdateLevelMenu(input);
    if (input->enter)
    {
        switch (levelIndex)
        {
            case 1: // LEVEL 1
                game->selectedStartLevel = 1;
                game->state = MENU;
                needRedraw = 1;
                return;

            case 2: // LEVEL 2
                game->selectedStartLevel = 2;
                game->state = MENU;
                needRedraw = 1;
                return;
        }
    }

    if (!levelNeedRedraw)
        return;

    ucg_ClearScreen(&ucg);
    ucg_SetFont(&ucg, ucg_font_helvR08_tf);

    ucg_DrawString(&ucg, 30, 40, 0, "Select Level");

    if (levelIndex == 1)
        ucg_DrawString(&ucg, 30, 70, 0, "> Level 1");
    else
        ucg_DrawString(&ucg, 30, 70, 0, "  Level 1");

    if (levelIndex == 2)
        ucg_DrawString(&ucg, 30, 84, 0, "> Level 2");
    else
        ucg_DrawString(&ucg, 30, 84, 0, "  Level 2");

    levelNeedRedraw = 0;
}

static const uint8_t dinoSprite[DINO_HEIGHT][DINO_WIDTH] =
{
    {0,0,0,0,0,1,1,1,1,1},
    {0,0,0,0,0,1,0,1,1,1},
    {0,0,0,0,0,1,1,1,0,0},
    {0,0,0,0,0,1,1,1,1,1},
    {0,0,0,0,1,1,1,1,0,0},
    {1,0,0,1,1,1,1,1,0,0},
    {1,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,1,0,0},
    {0,0,1,1,1,1,1,0,0,0},
    {0,0,1,1,1,1,1,0,0,0},
	{0,0,1,0,0,1,0,0,0,0},
	{0,0,1,1,0,1,1,0,0,0},
};

static void DrawDino(const Dino* dino)
{
    int baseX = dino->x;
    int baseY = dino->y;

    for (int y = 0; y < DINO_HEIGHT; y++)
    {
        for (int x = 0; x < DINO_WIDTH; x++)
        {
            if (dinoSprite[y][x])
            {
                ucg_DrawBox(
                    &ucg,
                    baseX + x * PIXEL_SCALE,
                    baseY + y * PIXEL_SCALE,
                    PIXEL_SCALE,
                    PIXEL_SCALE
                );
            }
        }
    }
}

static const uint8_t cactusSprite[CACTUS_HEIGHT][CACTUS_WIDTH] =
{
    {0,0,1,0,0},
    {0,1,1,1,0},
    {0,1,1,1,0},
    {1,1,1,1,1},
    {0,0,1,0,0},
    {0,0,1,0,0},
    {0,0,1,0,0},
    {0,0,1,0,0},
};

static void DrawOneCactus(int baseX, int baseY)
{
    for (int y = 0; y < CACTUS_HEIGHT; y++)
    {
        for (int x = 0; x < CACTUS_WIDTH; x++)
        {
            if (cactusSprite[y][x])
            {
                ucg_DrawBox(
                    &ucg,
                    baseX + x * PIXEL_SCALE,
                    baseY + y * PIXEL_SCALE,
                    PIXEL_SCALE,
                    PIXEL_SCALE
                );
            }
        }
    }
}

static void DrawObstacles(const ObstacleManager* om)
{
    for (int i = 0; i < om->obstacleCount; i++)
    {
        const Obstacle* obs = &om->obstacles[i];
        if (!obs->active) continue;

        int width = CACTUS_WIDTH * PIXEL_SCALE * obs->count;
        int height = CACTUS_HEIGHT * PIXEL_SCALE;

        /* ===== CLEAR OBSTACLE CŨ ===== */
        ucg_SetColor(&ucg, 0, 0, 0, 0);
        ucg_DrawBox(
            &ucg,
            obs->prevX,
            obs->y,
            width,
            height
        );

        /* ===== DRAW OBSTACLE MỚI ===== */
        ucg_SetColor(&ucg, 0, 255, 255, 255);

        for (int k = 0; k < obs->count; k++)
        {
            int ox = obs->x + k * CACTUS_WIDTH * PIXEL_SCALE;
            DrawOneCactus(ox, obs->y);
        }
    }
}

static void DrawScoreLabel(void)
{
    ucg_SetFont(&ucg, ucg_font_helvR08_tf);
    ucg_SetColor(&ucg, 0, 255, 255, 255);

    ucg_DrawString(&ucg, 0, 10, 0, "Score:");
}

static void DrawScoreNumber(int score)
{
    int x = 42;   // sau chữ "Score:"
    int y = 10;

    /* ===== CLEAR SỐ CŨ ===== */
    if (!firstScoreFrame)
    {
        ucg_SetColor(&ucg, 0, 0, 0, 0);
        ucg_DrawBox(
            &ucg,
            prevScoreNumRect.x,
            prevScoreNumRect.y,
            prevScoreNumRect.w,
            prevScoreNumRect.h
        );
    }

    /* ===== DRAW SỐ MỚI ===== */
    ucg_SetColor(&ucg, 0, 255, 255, 255);
    DrawNumber(x, y, score);

    /* ===== ƯỚC LƯỢNG VÙNG SỐ ===== */
    int digits = (score == 0) ? 1 : 0;
    int tmp = score;
    while (tmp > 0)
    {
        digits++;
        tmp /= 10;
    }

    prevScoreNumRect.x = x;
    prevScoreNumRect.y = y - 8;     // font height
    prevScoreNumRect.w = digits * 6;
    prevScoreNumRect.h = 10;
}

static void RenderPlaying(Game* game)
{
    Dino* dino = Game_GetDino(game);
    Rect curr = Dino_GetHitBox(dino);

    if (firstScoreFrame)
    {
		DrawScoreLabel();
    	firstScoreFrame = 0;
    }

	DrawScoreNumber(Game_GetScore(game));

    /* Clear Dino cũ */
    if (!firstFrame) {
        ucg_SetColor(&ucg, 0, 0, 0, 0);   // màu nền
        ucg_DrawBox(&ucg,
            prevDinoRect.x,
            prevDinoRect.y,
            prevDinoRect.w,
            prevDinoRect.h
        );
    }

    /* Ground (vẽ 1 lần hoặc vẽ đè) */
    ucg_SetColor(&ucg, 0, 255, 255, 255);
//    ucg_DrawHLine(&ucg, 0, GROUND_Y, SCREEN_WIDTH);
    if (firstFrame)
		ucg_DrawHLine(&ucg, 0, GROUND_Y, SCREEN_WIDTH);

    /* Draw Dino mới */
    DrawDino(dino);

    /* Draw Obstacle */
    DrawObstacles(Game_GetObstacleManager(game));

    prevDinoRect = curr;
    firstFrame = 0;
}

static void RenderGameOver(const GameInput* input, Game* game)
{
    if (gameOverNeedRedraw)
    {
        ucg_ClearScreen(&ucg);
        ucg_SetFont(&ucg, ucg_font_helvR08_tf);

        ucg_DrawString(&ucg, 30, 50, 0, "GAME OVER");

        gameOverNeedRedraw = 0;
    }

    /* ENTER → QUAY VỀ MENU */
    if (input->enter)
    {
        game->state = MENU;
        needRedraw = 1;          // menu vẽ lại
    }
}

static GameInput ReadGameInput(void)
{
    static GameInput prev = {0};
    GameInput curr;
    GameInput out;

    /* đọc trạng thái hiện tại (level) */
    curr.down  = (ButtonRead_Status(BUTTON1_GPIO_PORT, BUTTON_B5_PIN) == GPIO_PIN_LOW);
    curr.up    = (ButtonRead_Status(BUTTON2_GPIO_PORT, BUTTON_B1_PIN) == GPIO_PIN_LOW);
    curr.enter = (ButtonRead_Status(BUTTON3_GPIO_PORT, BUTTON_B3_PIN) == GPIO_PIN_LOW);
    curr.left  = (ButtonRead_Status(BUTTON4_GPIO_PORT, BUTTON_B2_PIN) == GPIO_PIN_LOW);
    curr.right = (ButtonRead_Status(BUTTON5_GPIO_PORT, BUTTON_B4_PIN) == GPIO_PIN_LOW);

    /* edge detect: chỉ true khi vừa nhấn */
    out.down  = curr.down  && !prev.down;
    out.up    = curr.up    && !prev.up;
    out.enter = curr.enter && !prev.enter;
    out.left  = curr.left  && !prev.left;
    out.right = curr.right && !prev.right;

    prev = curr;
    return out;
}

static
void AppInitCommon(void)
{
	//System 84MHz-------------------------------------------------------------
	SystemCoreClockUpdate();

	//Button Init--------------------------------------------------------------
	Button_Init();

	//Buzzer Init--------------------------------------------------------------
	Buzzer_Init();
}

///**
// * @func   BuzzerControl_SetStatus
// * @brief  Control Turn on or Turn off Buzzer
// * @param  None
// * @retval None
// */
static
void BuzzerControl_SetStatus(GPIO_TypeDef * GPIOx, uint8_t GPIO_PIN, uint8_t Status)
{

	//Set bit in BSRR Registers------------------------------------------------
	if (Status == GPIO_PIN_SET)
	{
		GPIOx->BSRRL |= 1 << GPIO_PIN;
	}
	//Reset bit in BSRR Registers----------------------------------------------
	if (Status == GPIO_PIN_RESET)
	{
		GPIOx->BSRRH |= 1 << GPIO_PIN;
	}
}

///**
// * @func   ButtonRead_Status
// * @brief  Read Status Button
// * @param  None
// * @retval None
// */
static
uint8_t ButtonRead_Status(GPIO_TypeDef * GPIOx, uint32_t GPIO_PIN)
{
	uint32_t Read_Pin;

	//Read bit in IDR Registers------------------------------------------------
	Read_Pin = (GPIOx->IDR) >> GPIO_PIN;
	Read_Pin = Read_Pin & 0x01;

	return Read_Pin;
}

///**
// * @func   Delay Time
// * @brief  Delay
// * @param  None
// * @retval None
// */
static
void Delay(uint32_t ms)
{
	uint32_t i,j;
	for (i = 0 ; i < ms ; i ++)
	{
		for (j = 0; j<5000; j++){;}
	}
}

///**
// * @func   Initializes GPIO Use Buzzer
// * @brief  Buzzer_Init
// * @param  None
// * @retval None
// */
//static
void Buzzer_Init(void)
{
	//Declare type variable GPIO Struc-----------------------------------------
	GPIO_InitTypeDef GPIO_InitStructure;

	//Enable Clock GPIOC-------------------------------------------------------
	RCC_AHB1PeriphClockCmd(BUZZERControl_SetClock, ENABLE);

	//Choose Pin Buzzer--------------------------------------------------------
	GPIO_InitStructure.GPIO_Pin = BUZZER_GPIO_PIN;

	//Choose Pin Buzzer as Out-------------------------------------------------
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	//Choose Speed Pin---------------------------------------------------------
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	//Select type--------------------------------------------------------------
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	//Select status------------------------------------------------------------
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;

	//The function initializes all of the above values-------------------------
	GPIO_Init(BUZZER_GPIO_PORT, &GPIO_InitStructure);
}

///**
// * @func   Initializes GPIO Use Button
// * @brief  ButtonInit
// * @param  None
// * @retval None
// */
//static
void Button_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure ;

    /* ========= BUTTON PB4 & PB5 ========= */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_0 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* ========= BUTTON PA4 ========= */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;

    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/* END_FILE */
