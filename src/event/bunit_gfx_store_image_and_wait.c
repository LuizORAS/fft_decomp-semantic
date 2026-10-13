#include "psx/libgpu.h"

/* Copy the VRAM rectangle `rect` to `data` (StoreImage) and wait until the GPU is idle. */
void bunit_gfx_store_image_and_wait(RECT* rect, void* data) {
    StoreImage(rect, data);
    while (DrawSync(1) != 0) { }
}
