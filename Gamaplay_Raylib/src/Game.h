#pragma once
#include "raylib.h"
#include "Dino.h"
#include "Obstacle.h" // Đã đổi tên theo yêu cầu của bạn

// Định nghĩa các trạng thái của game
enum GameState {
    MENU,
    PLAYING,
    HIGHSCORE_VIEW, // Thêm trạng thái xem điểm cao
    GAME_OVER
};

enum MenuOption {
    OPT_START,
    OPT_LEVEL,
    OPT_HIGHSCORE,
    OPT_EXIT,
    COUNT_OPT // Để đếm tổng số mục
};

class Game {
private:
    // Các thành phần con
    Dino dino;
    ObstacleManager obstacleManager; // Class quản lý cây (từ file Obstacle.h)

    // Biến trạng thái game
    GameState state;
    float score;
    int highScore;
    int currentLevel;

    int selectedOption; // Đang chọn mục nào (0, 1, 2, 3)
    int selectedStartLevel; // Level người chơi chọn ở menu (1 hoặc 2)
    bool shouldClose; // Biến kiểm tra để thoát game

    // Các hàm nội bộ (Helper functions)
    void Update();       // Tính toán logic
    void Draw();         // Vẽ hình
    void ResetGame();    // Đặt lại dữ liệu khi chơi lại
    bool CheckCollision(); // Kiểm tra va chạm

    void UpdateMenu();
    void DrawMenu();

public:
    Game();  // Constructor (Khởi tạo cửa sổ)
    ~Game(); // Destructor (Đóng cửa sổ)

    void Run(); // Hàm chạy vòng lặp chính
};