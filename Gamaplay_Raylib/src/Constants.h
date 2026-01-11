#pragma once
#include "raylib.h"

// --- CẤU HÌNH CỬA SỔ (WINDOW) ---
// Thêm từ khóa "static" vào trước để tránh lỗi Multiple Definition
static const int SCREEN_WIDTH = 800;
static const int SCREEN_HEIGHT = 450;

static const int TARGET_FPS = 60; 

// --- MÀU SẮC (THEME) ---
static const Color COLOR_BACKGROUND = RAYWHITE; 
static const Color COLOR_DINO = DARKGRAY;      
static const Color COLOR_CACTUS = DARKGRAY;

// --- VẬT LÝ & LOGIC ---
static const int GROUND_Y = 350;     
static const float GRAVITY = 0.8f;   
static const float JUMP_FORCE = -14.0f; 

static const int SPEED_LEVEL_1 = 6;
static const int SPEED_LEVEL_2 = 10; 
static const int LEVEL_UP_SCORE = 100;

// --- ĐỒ HỌA PIXEL ---
static const int PIXEL_SCALE = 4;