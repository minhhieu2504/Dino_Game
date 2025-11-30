# DINO RUNNER - C++ RAYLIB & EMBEDDED GUIDE

## 🎮 Hướng Dẫn Chơi (Gameplay)

### Mục tiêu
Điều khiển chú khủng long nhảy qua các cây xương rồng để đạt điểm số cao nhất. Tốc độ game sẽ tăng dần theo Level.

### Điều khiển
* **SPACE** hoặc **Mũi tên Lên (↑)**: Nhảy.
* **Mũi tên Lên / Xuống / Trái / Phải**: Di chuyển lựa chọn trong Menu.
* **ENTER**: Chọn mục trong Menu / Bắt đầu game / Chơi lại.

### Các Tính Năng
1.  **Menu System:** Giao diện chọn Start, chỉnh Level, xem High Score.
2.  **Level System:**
    * **Level 1:** Tốc độ bình thường.
    * **Level 2:** Tốc độ cao, thử thách phản xạ.
3.  **High Score:** Tự động lưu điểm cao nhất trong phiên chơi.
4.  **Pixel Art Graphics:** Đồ họa được vẽ trực tiếp bằng code (không phụ thuộc file ảnh ngoài).

---

## 💻 Cài đặt & Chạy trên PC (VS Code)

Dự án này sử dụng **Visual Studio Code** và **Raylib Starter Template**.

### Yêu cầu
* Visual Studio Code.
* Trình biên dịch C++ (MinGW/GCC) đi kèm bộ w64devkit của Raylib.

### Cách biên dịch (Build)
1.  Mở thư mục dự án trong VS Code.
2.  Nhấn tổ hợp phím `Ctrl + Shift + B`.
3.  Chọn Task: **`build debug`** (để biên dịch toàn bộ các file `.cpp` trong thư mục `src`).
4.  Đợi thông báo "Build finished successfully".

### Cách chạy (Run)
* Nhấn **F5** để bắt đầu Debug và chơi game.
* Hoặc tìm file `Game.exe` trong thư mục gốc và chạy trực tiếp.

---

## 🛠️ Hướng dẫn Porting sang STM32F401 + LCD TFT

Phần này hướng dẫn cách chuyển mã nguồn game này để chạy trên vi điều khiển **STM32F401** (ví dụ: STM32F401CCU6 Black Pill) kết hợp với màn hình LCD TFT (ví dụ: ILI9341, ST7735).

### 1. Nguyên lý chuyển đổi
Thư viện **Raylib không chạy được trên STM32**. Do đó, chúng ta cần tách phần **Logic** (tính toán) và phần **Render** (hiển thị):

* **Giữ lại (Logic Game):** `Dino.cpp`, `Obstacle.cpp`, `Constants.h`. Các file này chỉ chứa toán học (cộng trừ tọa độ, kiểm tra va chạm), hoàn toàn tương thích với C++ trên STM32.
* **Thay thế (Render Layer):** File `Game.cpp` và các hàm vẽ của Raylib phải được thay thế bằng thư viện điều khiển màn hình (LCD Driver) của STM32.

### 2. Kết nối phần cứng (Gợi ý)
Sử dụng giao tiếp **SPI** để đạt tốc độ khung hình (FPS) tốt nhất.

| Chân LCD (ILI9341) | Chân STM32F401 (Gợi ý) | Chức năng |
| :--- | :--- | :--- |
| **VCC/GND** | 3.3V / GND | Nguồn |
| **CS** | PA4 | Chip Select |
| **RESET** | PA2 | Reset màn hình |
| **DC/RS** | PA3 | Data/Command |
| **MOSI** | PA7 (SPI1_MOSI) | Gửi dữ liệu |
| **SCK** | PA5 (SPI1_SCK) | Xung nhịp |
| **LED** | 3.3V | Đèn nền |

### 3. Quy trình thực hiện phần mềm (Software)

#### Bước 1: Chuẩn bị Project STM32
* Dùng **STM32CubeIDE** hoặc **Keil C**.
* Cấu hình **SPI1** (Transmit Only Master).
* Thêm thư viện điều khiển màn hình (Ví dụ: thư viện `ILI9341_STM32_Driver` hoặc `ST7735`).

#### Bước 2: Thay thế hàm đồ họa (Mapping)
Trong các file `.cpp`, bạn cần thay đổi các hàm Raylib thành hàm của LCD Driver:

| Chức năng | Raylib (Code cũ) | STM32 (Code mới - Ví dụ ILI9341) |
| :--- | :--- | :--- |
| **Màu sắc** | `RED`, `GREEN` | `ILI9341_RED`, `ILI9341_GREEN` (Mã Hex 16-bit) |
| **Vẽ khối** | `DrawRectangle(x, y, w, h, c)` | `ILI9341_FillRectangle(x, y, w, h, c)` |
| **Xóa màn hình** | `ClearBackground(WHITE)` | `ILI9341_FillScreen(ILI9341_WHITE)` |
| **Viết chữ** | `DrawText("Score", x, y, ...)` | `ILI9341_WriteString(x, y, "Score", Font_11x18, ...)` |
| **Nút bấm** | `IsKeyPressed(KEY_UP)` | `if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 1)` |

#### Bước 3: Cấu trúc vòng lặp chính (Main Loop)
Trên STM32 không có `SetTargetFPS`. Bạn dùng `HAL_Delay` để kiểm soát tốc độ game.

```cpp
// Trong main.c của STM32
while (1) {
    // 1. Xóa màn hình cũ (Hoặc xóa cục bộ vật thể để tối ưu)
    // Clear_Dino_Old_Pos();

    // 2. Cập nhật Logic (Giữ nguyên code C++ logic)
    dino.Update();
    obstacleManager.Update(currentLevel);

    // 3. Vẽ mới (Dùng hàm LCD)
    dino.Draw_On_LCD(); 
    obstacleManager.Draw_On_LCD();

    // 4. Delay (Thay thế cho FPS)
    HAL_Delay(30); // ~30 FPS
}
