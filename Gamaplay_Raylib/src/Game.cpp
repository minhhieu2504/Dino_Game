#include "Game.h"
#include "Constants.h"
#include <stdio.h> // Để dùng sprintf vẽ điểm số

Game::Game() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Dino Runner - C++ Raylib");
    SetTargetFPS(TARGET_FPS);

    // Khởi tạo các giá trị ban đầu
    state = MENU;
    score = 0;
    highScore = 0;
    currentLevel = 1;

    // Khởi tạo giá trị Menu
    selectedOption = OPT_START;
    selectedStartLevel = 1; // Mặc định chọn level 1
    shouldClose = false;
}

Game::~Game() {
    CloseWindow(); // Giải phóng bộ nhớ cửa sổ khi tắt game
}

void Game::ResetGame() {
    state = PLAYING;
    score = 0;
    currentLevel = selectedStartLevel;
    
    // Gọi hàm reset của các đối tượng con
    // Dino cần reset vị trí, Obstacle cần xóa hết cây cũ
    dino = Dino(); // Tạo mới Dino (về vị trí cũ)
    obstacleManager.Reset();
}

bool Game::CheckCollision() {
    // Thu nhỏ hitbox của Dino
    Rectangle dinoRect = dino.GetRect();
    dinoRect.x += 4;      
    dinoRect.width -= 8;  
    dinoRect.y += 4;      
    dinoRect.height -= 8; 

    for (const auto& obs : obstacleManager.obstacles) {
        
        // --- TÍNH TOÁN HITBOX CHO NHÓM CÂY ---
        // Chiều rộng = Chiều rộng 1 cây * Số lượng cây
        float totalWidth = (float)CACTUS_WIDTH * PIXEL_SCALE * obs.count;

        Rectangle obsRect = {
            obs.x, 
            obs.y, 
            totalWidth, // Chiều rộng mở rộng theo số lượng
            (float)CACTUS_HEIGHT * PIXEL_SCALE 
        };
        
        // Thu nhỏ hitbox cây
        obsRect.x += 4;
        obsRect.width -= 8;

        if (CheckCollisionRecs(dinoRect, obsRect)) {
            return true; 
        }
    }
    return false;
}

void Game::Update() {
    // Nếu biến exit bật lên -> Đóng cửa sổ (Thoát game)
    if (shouldClose) {
        CloseWindow();
        return;
    }

    switch (state) {
        case MENU:
            UpdateMenu();
            break;

        case PLAYING:
            dino.Update();
            obstacleManager.Update(currentLevel);
            score += 0.1f;

            if (CheckCollision()) {
                state = GAME_OVER;
                if (score > highScore) highScore = (int)score;
            }
            break;

        case HIGHSCORE_VIEW:
            // Bấm phím bất kỳ để quay lại Menu
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
                state = MENU;
            }
            break;

        case GAME_OVER:
            if (IsKeyPressed(KEY_ENTER)) {
                state = MENU; // Chết xong quay về Menu thay vì chơi lại ngay
            }
            break;
    }
}

void Game::Draw() {
    BeginDrawing();
    ClearBackground(COLOR_BACKGROUND);
    
    // Luôn vẽ đất
    DrawLine(0, GROUND_Y, SCREEN_WIDTH, GROUND_Y, DARKGRAY);

    switch (state) {
        case MENU:
            DrawMenu();
            break;

        case PLAYING:
            dino.Draw();
            obstacleManager.Draw();
            DrawText(TextFormat("Score: %04i", (int)score), 20, 20, 20, DARKGRAY);
            DrawText(TextFormat("Lv: %i", currentLevel), 20, 50, 20, DARKGRAY);
            break;

        case HIGHSCORE_VIEW:
            DrawText("HALL OF FAME", 250, 100, 40, DARKGRAY);
            DrawText(TextFormat("High Score: %05i", highScore), 280, 200, 30, GOLD);
            DrawText("Press ENTER to Return", 280, 350, 20, LIGHTGRAY);
            break;

        case GAME_OVER:
            dino.Draw();
            obstacleManager.Draw();
            DrawText("GAME OVER", 260, 150, 50, RED);
            DrawText(TextFormat("Score: %i", (int)score), 340, 220, 20, DARKGRAY);
            DrawText("Press ENTER to Menu", 280, 280, 20, DARKGRAY);
            break;
    }

    EndDrawing();
}

void Game::UpdateMenu() {
    // 1. Di chuyển LÊN / XUỐNG
    if (IsKeyPressed(KEY_DOWN)) {
        selectedOption++;
        if (selectedOption >= COUNT_OPT) selectedOption = 0; // Vòng lại đầu
    }
    if (IsKeyPressed(KEY_UP)) {
        selectedOption--;
        if (selectedOption < 0) selectedOption = COUNT_OPT - 1; // Vòng xuống cuối
    }

    // 2. Chọn (Bấm ENTER)
    if (IsKeyPressed(KEY_ENTER)) {
        switch (selectedOption) {
            case OPT_START:
                ResetGame();
                break;
            
            case OPT_LEVEL:
                // Bấm vào đây để đổi Level 1 <-> 2
                if (selectedStartLevel == 1) selectedStartLevel = 2;
                else selectedStartLevel = 1;
                break;

            case OPT_HIGHSCORE:
                state = HIGHSCORE_VIEW;
                break;

            case OPT_EXIT:
                shouldClose = true; // Báo hiệu thoát vòng lặp
                break;
        }
    }
    
    // Logic phụ: Cho phép bấm phím Trái/Phải để chỉnh Level nhanh
    if (selectedOption == OPT_LEVEL) {
        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)) {
             if (selectedStartLevel == 1) selectedStartLevel = 2;
             else selectedStartLevel = 1;
        }
    }
}

void Game::DrawMenu() {
    DrawText("DINO RUNNER", 220, 80, 50, DARKGRAY);

    // Tọa độ vẽ menu
    int menuX = 300;
    int menuY = 180;
    int spacing = 40; // Khoảng cách giữa các dòng

    // Vẽ từng dòng
    // 1. START
    Color c = (selectedOption == OPT_START) ? RED : LIGHTGRAY;
    DrawText("START GAME", menuX, menuY, 20, c);

    // 2. LEVEL
    c = (selectedOption == OPT_LEVEL) ? RED : LIGHTGRAY;
    // Vẽ chữ "Level: 1" hoặc "Level: 2"
    DrawText(TextFormat("LEVEL: < %d >", selectedStartLevel), menuX, menuY + spacing, 20, c);

    // 3. HIGHSCORE
    c = (selectedOption == OPT_HIGHSCORE) ? RED : LIGHTGRAY;
    DrawText("HIGH SCORE", menuX, menuY + spacing * 2, 20, c);

    // 4. EXIT
    c = (selectedOption == OPT_EXIT) ? RED : LIGHTGRAY;
    DrawText("EXIT", menuX, menuY + spacing * 3, 20, c);

    // Hướng dẫn
    DrawText("Use ARROW KEYS to move, ENTER to select", 200, 400, 15, GRAY);
}

void Game::Run() {
    // Vòng lặp chạy khi: Cửa sổ chưa đóng VÀ Biến shouldClose là false
    while (!WindowShouldClose() && !shouldClose) {
        Update();
        // Kiểm tra lại lần nữa vì nếu Update đã gọi CloseWindow thì không vẽ nữa
        if (!WindowShouldClose() && !shouldClose) { 
            Draw();
        }
    }
}