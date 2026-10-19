#include "fft/gfx.h"

/* Copy the VRAM rectangle `rect` to (x, y) in VRAM (MoveImage) and wait until the GPU is idle. */
void gfx_move_image_and_wait(RECT* rect, s16 x, s16 y) {
    MoveImage(rect, x, y);
    while (DrawSync(1) != 0) { }
}
