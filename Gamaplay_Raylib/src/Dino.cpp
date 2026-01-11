#include "Dino.h"

// --- DỮ LIỆU HÌNH ẢNH (SPRITE DATA) ---
// 1: Vẽ màu, 0: Trống
// Dáng Dino Cute (15x16)
static int dinoSprite[DINO_HEIGHT][DINO_WIDTH] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1}, // Đầu (đỉnh)
    {0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,1,1}, // Mắt (số 0 ở giữa)
    {0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0}, // Mõm dưới
    {0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0},
    {1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0}, // Cổ
    {1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0},
    {0,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0}, // Thân trên & Tay
    {0,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
    {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0}, // Thân giữa
    {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
    {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
    {0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0}, // Bụng & Đùi
    {0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0}, // Đuôi
    {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0}, // Chân trên
    {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0}, // Cẳng chân
    {0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,1,0,1,1,1,0,0,0,0,0,0}  // Bàn chân
};

Dino::Dino() {
    // Vị trí xuất phát
    x = 50;
    // Tính toán để chân chạm đúng mặt đất (GROUND_Y)
    y = GROUND_Y - (DINO_HEIGHT * PIXEL_SCALE); 
    velocityY = 0;
    isJumping = false;
}

void Dino::Update() {
    // 1. Xử lý nhảy (Input)
    if (IsKeyPressed(KEY_SPACE) && !isJumping) {
        velocityY = JUMP_FORCE;
        isJumping = true;
    }

    // 2. Vật lý (Trọng lực)
    y += velocityY;
    
    // Nếu đang trên không thì bị kéo xuống
    if (isJumping || y < GROUND_Y - (DINO_HEIGHT * PIXEL_SCALE)) {
        velocityY += GRAVITY;
    }

    // 3. Kiểm tra chạm đất
    int groundLimit = GROUND_Y - (DINO_HEIGHT * PIXEL_SCALE);
    if (y >= groundLimit) {
        y = groundLimit;
        velocityY = 0;
        isJumping = false;
    }
}

void Dino::Draw() {
    // VẼ PIXEL ART
    // Duyệt qua từng dòng và cột của mảng dinoSprite
    for (int row = 0; row < DINO_HEIGHT; row++) {
        for (int col = 0; col < DINO_WIDTH; col++) {
            
            if (dinoSprite[row][col] == 1) {
                // Tính tọa độ vẽ trên màn hình
                // Tọa độ vẽ = Vị trí Dino + (Vị trí ô * Tỉ lệ phóng to)
                int drawX = (int)x + (col * PIXEL_SCALE);
                int drawY = (int)y + (row * PIXEL_SCALE);

                // Vẽ ô vuông đại diện cho 1 pixel
                DrawRectangle(drawX, drawY, PIXEL_SCALE, PIXEL_SCALE, COLOR_DINO);
            }
        }
    }
}

Rectangle Dino::GetRect() {
    // Trả về hình chữ nhật bao quanh Dino để tính va chạm
    return Rectangle{ x, y, (float)DINO_WIDTH * PIXEL_SCALE, (float)DINO_HEIGHT * PIXEL_SCALE };
}