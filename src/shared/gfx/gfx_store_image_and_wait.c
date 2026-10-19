#include "fft/gfx.h"

/* Copy the VRAM rectangle `rect` to `data` (StoreImage) and wait until the GPU is idle. */
void gfx_store_image_and_wait(RECT* rect, void* data) {
    StoreImage(rect, data);
    while (DrawSync(1) != 0) { }
}
