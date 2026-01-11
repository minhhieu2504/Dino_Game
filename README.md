# 🦖 Dino Game – STM32F401RE

Dino Game là một game đơn giản được xây dựng trên **STM32F401RE (NUCLEO-F401RE)**, mô phỏng trò chơi Dino quen thuộc (nhảy tránh vật cản), sử dụng **Ucglib** để hiển thị LCD, **button vật lý** để điều khiển và **buzzer** để phản hồi âm thanh.

---

## 📌 Phần cứng sử dụng

- **Board:** NUCLEO-F401RE (STM32F401RE – Cortex-M4)
- **LCD:** LCD sử dụng thư viện **Ucglib**
- **Button:** 5 nút nhấn (UP / DOWN / LEFT / RIGHT / ENTER)
- **Buzzer:** GPIO output (báo hiệu bắt đầu chơi & Game Over)

---

## 🧩 Chức năng chính

- Menu chính:
  - Start Game
  - Select Level
  - High Score
- Chọn Level:
  - Level 1
  - Level 2
- Gameplay:
  - Dino nhảy tránh chướng ngại vật
  - Tính điểm theo thời gian chơi
- High Score:
  - Lưu và hiển thị điểm cao nhất
- Game Over:
  - Hiển thị màn hình Game Over
  - Quay lại Menu bằng nút ENTER
- Âm thanh:
  - Buzzer kêu khi:
    - Bắt đầu chơi
    - Game Over

---

## 🎮 Điều khiển (Button Mapping)

| Nút | Chức năng |
|---|---|
| UP | Di chuyển lên trong menu |
| DOWN | Di chuyển xuống trong menu |
| ENTER | Chọn / Nhảy |
| LEFT | Dự phòng |
| RIGHT | Dự phòng |

> Các nút được đọc theo **edge detect** (chỉ kích hoạt khi vừa nhấn).

---

## 🗂️ Cấu trúc chương trình

### `main.c`
- Khởi tạo hệ thống:
  - Clock
  - Timer
  - Button
  - Buzzer
  - LCD
- Vòng lặp chính:
  - Đọc input
  - Cập nhật trạng thái game
  - Render theo từng state
- Quản lý State:
  - `MENU`
  - `LEVEL_MENU`
  - `PLAYING`
  - `GAME_OVER`
  - `HIGHSCORE_VIEW`

### Các module chính
- **Game**
  - Quản lý trạng thái game
  - Điểm số, level, va chạm
- **Dino**
  - Vị trí, hitbox, nhảy
- **Obstacle**
  - Sinh vật cản
  - Di chuyển và va chạm
- **Display**
  - Vẽ menu, gameplay, score, game over
- **Button**
  - Đọc trạng thái nút nhấn
- **Buzzer**
  - Phát âm báo hiệu

---

## 🖥️ Hiển thị

- Sử dụng **Ucglib**
- Font: `ucg_font_helvR08_tf`
- Vẽ thủ công từng pixel:
  - Dino sprite
  - Cactus sprite
- Chỉ vẽ lại vùng cần thiết để tối ưu hiệu năng

---

## 🔊 Buzzer

Buzzer được điều khiển bằng GPIO:

- **Bật buzzer** khi:
  - Chuyển sang trạng thái `PLAYING`
  - Chuyển sang trạng thái `GAME_OVER`
- Thời gian kêu: ~100ms

---

## ⚙️ Build & Toolchain

- **IDE:** STM32CubeIDE
- **Compiler:** arm-none-eabi-gcc
- **Chuẩn C:** GNU11
- **Linker script:** STM32F401RETx_FLASH.ld

---

## 🚀 Cách chạy

1. Import project vào **STM32CubeIDE**
2. Kiểm tra:
   - Linker script không bị exclude
   - Include path đầy đủ
3. Build project
4. Flash lên board NUCLEO-F401RE
5. Kết nối LCD, button, buzzer
6. Chạy và chơi 🎉

---

## 📷 Preview

![Preview](preview.jpg)

---

## 📌 Ghi chú

- Project tập trung vào:
  - Quản lý state
  - Vẽ LCD thủ công
  - Lập trình embedded cơ bản
- Phù hợp cho:
  - Sinh viên
  - Học STM32
  - Demo game embedded

---

## 👤 Tác giả

- **minhhieu2504**
- Dino Game – Embedded STM32 Project
