#include "Obstacle.h"
#include <cstdlib>

// --- DỮ LIỆU HÌNH ẢNH CÂY ---
static int cactusSprite[CACTUS_HEIGHT][CACTUS_WIDTH] = {
    {0,0,1,1,0,0}, // Đỉnh đầu
    {0,1,1,1,1,0}, 
    {1,1,1,1,1,1}, // Nhánh ngang cao
    {1,1,1,1,1,1}, 
    {1,1,1,1,1,1}, // Nhánh tách ra
    {0,0,1,1,0,0},
    {0,0,1,1,0,0}, // Bắt đầu thân (Trunk)
    {0,0,1,1,0,0},
    {0,0,1,1,0,0},
    {0,0,1,1,0,0}, // --- Phần thân được kéo dài thêm ở đây ---
    {0,0,1,1,0,0},
    {0,0,1,1,0,0},
    {0,0,1,1,0,0},
    {0,0,1,1,0,0}  // Gốc cây
};

ObstacleManager::ObstacleManager() {
    spawnTimer = 0;
    Reset();
}

void ObstacleManager::Reset() {
    obstacles.clear();
    spawnTimer = 0;
}

void ObstacleManager::Update(int currentLevel) {
    // 1. TỐC ĐỘ (Đã kiểm tra lại logic: Level 2 nhanh hơn)
    int currentSpeed = (currentLevel == 1) ? SPEED_LEVEL_1 : SPEED_LEVEL_2;

    // 2. DI CHUYỂN
    for (int i = 0; i < (int)obstacles.size(); i++) {
        obstacles[i].x -= currentSpeed;
        if (obstacles[i].x < -100) { // Sửa thành -100 để chắc chắn nhóm 3 cây đã đi hết
            obstacles[i].active = false;
        }
    }

    for (int i = 0; i < (int)obstacles.size(); ) {
        if (!obstacles[i].active) obstacles.erase(obstacles.begin() + i);
        else i++;
    }

    // 3. SINH CÂY MỚI VỚI TỈ LỆ NGẪU NHIÊN
    spawnTimer++;
    
    // Level 2 cây ra nhanh hơn một chút
    int spawnTimeThreshold = (currentLevel == 1) ? 90 : 70; 

    if (spawnTimer > spawnTimeThreshold) {
        if (rand() % 100 < 50) { 
            Obstacle newCactus;
            newCactus.x = SCREEN_WIDTH + 20;
            newCactus.y = GROUND_Y - (CACTUS_HEIGHT * PIXEL_SCALE);
            newCactus.active = true;

            // --- LOGIC TỈ LỆ XUẤT HIỆN 1-2-3 CÂY ---
            int r = rand() % 100; // Sinh số từ 0 đến 99
            
            if (currentLevel == 1) {
                // LEVEL 1: Dễ
                if (r < 80) newCactus.count = 1;      // 80% ra 1 cây
                else newCactus.count = 2;             // 20% ra 2 cây
                // Level 1 không có 3 cây
            } 
            else {
                // LEVEL 2: Khó
                if (r < 40) newCactus.count = 1;      // 40% ra 1 cây
                else if (r < 80) newCactus.count = 2; // 40% ra 2 cây
                else newCactus.count = 3;             // 20% ra 3 cây (Rất khó)
            }

            obstacles.push_back(newCactus);
            spawnTimer = 0;
        }
    }
}

void ObstacleManager::Draw() {
    for (const auto& obs : obstacles) {
        // VẼ LẶP LẠI DỰA TRÊN SỐ LƯỢNG (COUNT)
        for (int k = 0; k < obs.count; k++) {
            
            // Tính toán khoảng cách offset cho từng cây trong nhóm
            // Mỗi cây cách nhau đúng bằng chiều rộng của nó
            int offsetX = k * (CACTUS_WIDTH * PIXEL_SCALE); 

            // Vẽ từng pixel của từng cây
            for (int row = 0; row < CACTUS_HEIGHT; row++) {
                for (int col = 0; col < CACTUS_WIDTH; col++) {
                    if (cactusSprite[row][col] == 1) {
                        
                        int drawX = (int)obs.x + offsetX + (col * PIXEL_SCALE);
                        int drawY = (int)obs.y + (row * PIXEL_SCALE);

                        DrawRectangle(drawX, drawY, PIXEL_SCALE, PIXEL_SCALE, COLOR_CACTUS);
                    }
                }
            }
        }
    }
}