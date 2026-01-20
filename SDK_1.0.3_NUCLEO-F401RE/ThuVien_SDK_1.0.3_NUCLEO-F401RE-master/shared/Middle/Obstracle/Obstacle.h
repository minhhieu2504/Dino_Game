#pragma once
#include <stdint.h>
#include "Dino.h"

/* ===== CONFIG ===== */
#define MAX_OBSTACLES 6

#define CACTUS_WIDTH   5
#define CACTUS_HEIGHT  8

/* ===== SPEED ===== */
#define SPEED_LEVEL_1   13
#define SPEED_LEVEL_2   15

/* ===== Obstacle ===== */
/* x: vị trí hiện tại theo trục ngang - vẽ vật cản/ kiểm tra va chạm
 *
 * prevX: vị trí ở frame trước theo trục ngang - phát hiện vật cản vượt qua Dino/ tính score
 *
 *  y: vị trí theo trục dọc - cố định khi vật cản xuất hiện
 *
 *  count: số cây trong nhóm
 *
 *  active: cờ trạng thái vật cản (active = 1: vật cản đang spawn trên màn hình)
 */

typedef struct {
    int x;
    int prevX;
    int y;
    int count;
    uint8_t active;
} Obstacle;

/* ===== Obstacle Manager ===== */
/* obstacles[MAX_OBSTACLES]: mảng chứa obstacle tồn tại
 *
 * obstacleCount: sô obstacle đang xuất hiện - giới hạn độ khó/ tránh spawn quá nhiều
 *
 * spawnTimer: bộ đém số frame từ lần spawn trước - tạo khoảng cách giữa các obstacle
 */

typedef struct {
    Obstacle obstacles[MAX_OBSTACLES];
    int obstacleCount;
    int spawnTimer;
} ObstacleManager;

void ObstacleManager_Reset(ObstacleManager* mgr);
void ObstacleManager_Update(ObstacleManager* mgr, int currentLevel);

uint8_t ObstacleManager_CheckCollision(const ObstacleManager* mgr, const Dino* dino);
