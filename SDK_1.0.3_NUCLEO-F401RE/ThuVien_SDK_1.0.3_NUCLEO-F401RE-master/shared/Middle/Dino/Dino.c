#include "Dino.h"

int groundLimit = GROUND_Y - (DINO_HEIGHT * PIXEL_SCALE);

void Dino_Reset(Dino* dino)
{
    dino->x = 10;
    dino->y = groundLimit;
    dino->velocityY = 0;
    dino->isJumping = 0;
}

/*
 * Khi có input->jump && Dino đang chạm đất
 * -> velocityY = JUMP_FORCE -> dino->y giảm 1 đoạn velocityY (velocity = JUMP_FROCE = số âm)
 * -> Dino nhảy lên nhanh
 *
 * Khi Dino đang nhảy || Dino chưa chạm đất
 * -> velocityY tăng 1 đoạn GRAVITY -> velocityY + GRAVITY = số âm -> dino->y giảm 1 đoạn = velocityY
 * -> Dino nhảy chậm dần
 * -> Dino tiếp tục đang nhảy -> velocityY tiếp tục tăng -> dino->y tiếp tục giảm
 * -> Tại thời điểm velocityY + GRAVITY = số dương, dino->y tăng trở lại
 * -> Dino rơi xuống
 *
 * Khi dino->y tăng đến groundlimit -> vào trạng thái chạm đất
 */

void Dino_Update(Dino* dino, const DinoInput* input)
{
    /* Nhảy */
    if (input->jump && !dino->isJumping) {
        dino->velocityY = JUMP_FORCE;
        dino->isJumping = 1; // Đánh dấu Dino đã nhảy lên -> tránh Double Jump
    }

    /* Cập nhật vị trí */
    dino->y += dino->velocityY;

    /* Trọng lực */
    if (dino->isJumping || dino->y < groundLimit) {
        dino->velocityY += GRAVITY;
    }

    /* Chạm đất */
    if (dino->y >= groundLimit) {
        dino->y = groundLimit;
        dino->velocityY = 0;
        dino->isJumping = 0;
    }
}

Rect Dino_GetHitBox(const Dino* dino)
{
    Rect r;
    r.x = dino->x;
    r.y = dino->y;
    r.w = DINO_WIDTH * PIXEL_SCALE;
    r.h = DINO_HEIGHT * PIXEL_SCALE;
    return r;
}
