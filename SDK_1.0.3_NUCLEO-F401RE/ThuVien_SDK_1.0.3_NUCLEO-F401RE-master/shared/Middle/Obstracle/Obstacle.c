#include "Obstacle.h"
#include <stdlib.h>   // rand()

void ObstacleManager_Reset(ObstacleManager* mgr)
{
    mgr->obstacleCount = 0;
    mgr->spawnTimer = 0;

    for (int i = 0; i < MAX_OBSTACLES; i++)
        mgr->obstacles[i].active = 0;
}

void ObstacleManager_Update(ObstacleManager* mgr, int currentLevel)
{
    int speed = (currentLevel == 1) ? SPEED_LEVEL_1 : SPEED_LEVEL_2;

    /* ===== 1. MOVE ===== */
    for (int i = 0; i < mgr->obstacleCount; i++)
    {
        if (!mgr->obstacles[i].active) continue; // Bỏ qua vật cản chưa active

        mgr->obstacles[i].prevX = mgr->obstacles[i].x; // Lưu lại vị trí cũ
        mgr->obstacles[i].x -= speed; // di chuyển vật cản sang trái theo speed

        /* Vật cản di chuyển về bên trái màn hinh -> x tiếp tục giảm
         * Tại thời điểm x < CACTUS_WIDTH * PIXEL_SCALE * MAX_OBSTACLES - Khoảng cách của vật cản
         * (* MAX_OBSTACLES để đảm bảo là các vật cản nhóm (1...4) cây đã đi ra hết màn hình
         */

        if (mgr->obstacles[i].x <-(CACTUS_WIDTH * PIXEL_SCALE * MAX_OBSTACLES))
        {
            mgr->obstacles[i].active = 0;
        }
    }

    /* ===== 2. COMPACT ===== */
    /* Cập nhật lại danh sách mảng vật cản
     * Loại bỏ những vật cản có active = 0 -> cập nhật lại số lượng vật cản (obstacleCount)
     * -> Giảm thời gian kiểm tra active -> Update nhanh hơn
     */

    int writeIndex = 0;
    for (int i = 0; i < mgr->obstacleCount; i++)
    {
        if (mgr->obstacles[i].active)
            mgr->obstacles[writeIndex++] = mgr->obstacles[i];
    }
    mgr->obstacleCount = writeIndex;

    /* ===== 3. SPAWN ===== */
    mgr->spawnTimer++;

    int spawnThreshold = (currentLevel == 1) ? 18 : 12; // thời gian để spawn vật cản theo level

    if (mgr->spawnTimer >= spawnThreshold && mgr->obstacleCount < MAX_OBSTACLES)
    {
        Obstacle* obs = &mgr->obstacles[mgr->obstacleCount++];

        obs->x = SCREEN_WIDTH;
        obs->y = GROUND_Y -(CACTUS_HEIGHT * PIXEL_SCALE);
        obs->active = 1;

        int r = rand() % 100; // sinh ra random từ 0 đến 99 -> chia xác suất

        if (currentLevel == 1)
        {
        	if (r < 30)      obs->count = 1;
			else if (r < 70) obs->count = 2;
			else             obs->count = 3;
        }
        else
        {
            if (r < 20)      obs->count = 2;
            else if (r < 50) obs->count = 3;
            else             obs->count = 4;
        }

        // Sau khi đã sinh ra 1 vật cản thì reset bộ đếm thời gian chuẩn bị cho vật cản tiếp theo
        mgr->spawnTimer = 0;
    }
}

uint8_t ObstacleManager_CheckCollision(const ObstacleManager* mgr, const Dino* dino)
{
    Rect dinoRect = Dino_GetHitBox(dino);

    for (int i = 0; i < mgr->obstacleCount; i++) // Duyệt qua các vật cản đang active (sau khi đã lọc bằng COMPACT)
    {
        const Obstacle* obs = &mgr->obstacles[i];

        //Tạo hitbox cho vật cản
        Rect obsRect;
        obsRect.x = obs->x;
        obsRect.y = obs->y;
        obsRect.w = CACTUS_WIDTH * PIXEL_SCALE * obs->count;
        obsRect.h = CACTUS_HEIGHT * PIXEL_SCALE;

        if (dinoRect.x < obsRect.x + obsRect.w  &&
            dinoRect.x + dinoRect.w > obsRect.x &&
            dinoRect.y < obsRect.y + obsRect.h  &&
            dinoRect.y + dinoRect.h > obsRect.y)
        {
            return 1; // có va chạm
        }
    }
    return 0; // không có va chạm
}
