#pragma once
#include "raylib.h"
#include <vector>     // Thư viện vector để dùng mảng động
#include "Constants.h"

const int CACTUS_HEIGHT = 14; 
const int CACTUS_WIDTH = 6;

// Cấu trúc đại diện cho 1 cái cây
struct Obstacle {
    float x, y;
    bool active; // Đánh dấu xem cây còn dùng được không
    int count; // MỚI: Số lượng cây trong nhóm này (1, 2 hoặc 3)
};

class ObstacleManager {
private:
    // Biến đếm thời gian hoặc khoảng cách để sinh cây mới
    float spawnTimer; 
    
public:
    // Danh sách các cây đang có trên màn hình
    std::vector<Obstacle> obstacles;

    ObstacleManager();

    // Hàm cập nhật vị trí cây (speed thay đổi theo Level)
    void Update(int currentLevel);

    // Hàm vẽ toàn bộ cây
    void Draw();

    // Hàm reset lại từ đầu (khi Game Over)
    void Reset();
};