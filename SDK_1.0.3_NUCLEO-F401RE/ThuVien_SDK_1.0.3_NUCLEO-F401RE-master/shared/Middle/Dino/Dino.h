#pragma once
#include <stdint.h>

/* ===== SCREEN CONFIG ===== */
#define SCREEN_WIDTH   128
#define SCREEN_HEIGHT  128

#define GROUND_Y       104
#define PIXEL_SCALE    2

/* ===== DINO SIZE ===== */
#define DINO_WIDTH      10
#define DINO_HEIGHT    	12

/* ===== PHYSICS ===== */
#define JUMP_FORCE     -14
#define GRAVITY         3

/* ===== Hitbox ===== */
typedef struct {
    int x;
    int y;
    int w;
    int h;
} Rect;

/* ===== Input ===== */
typedef struct {
    uint8_t jump;
} DinoInput;

/* ===== Dino ===== */
/* x: vị trí theo trục ngang - cố định
 *
 * y: vị trí theo trục dọc
 * 	- Nhảy -> y giảm
 * 	- Rơi -> y tăng
 *
 *  velocityY: vận tốc theo trục Y - tạo chuyển động mượt
 *   - Nhảy -> velocityY âm
 *   - Rơi -> velocityY dương
 *
 *  isJumping: cờ trạng thái nhảy cho Dino - Nhảy = 1
 */

typedef struct {
    int x;
    int y;
    int velocityY;
    uint8_t isJumping;
} Dino;

void Dino_Reset(Dino* dino);
void Dino_Update(Dino* dino, const DinoInput* input);
Rect Dino_GetHitBox(const Dino* dino);
