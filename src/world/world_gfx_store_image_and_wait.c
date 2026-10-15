#include "psx/libgpu.h"
#include "psx/types.h"

/* Copy the VRAM rectangle `rect` to `data` (StoreImage) and wait until the GPU is idle. */
void world_gfx_store_image_and_wait(RECT* rect, void* dest) {
    StoreImage(rect, dest);
    while (DrawSync(1) != 0) { }
}
