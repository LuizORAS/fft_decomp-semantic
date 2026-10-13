#include "psx/libgpu.h"
#include "psx/types.h"

/* Upload `data` to the VRAM rectangle `rect` (LoadImage) and wait until the GPU is idle. */
void world_gfx_load_image_and_wait(RECT* rect, void* data) {
    LoadImage(rect, data);
    while (DrawSync(1) != 0) { }
}
