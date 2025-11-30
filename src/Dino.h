#pragma once
#include "raylib.h"
#include "Constants.h"

const int DINO_WIDTH = 20;
const int DINO_HEIGHT = 24;

class Dino {
public:
    float x, y;
    float velocityY; // Tốc độ rơi/nhảy
    bool isJumping;

    // Constructor
    Dino();

    // Hàm cập nhật vật lý (nhảy, rơi)
    void Update();

    // Hàm vẽ (sẽ dùng vòng lặp để vẽ các ô vuông)
    void Draw();

    // Hàm lấy vùng va chạm (để check đụng cây)
    Rectangle GetRect();
};