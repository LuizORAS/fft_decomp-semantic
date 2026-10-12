#include "fft/world.h"
#include "psx/libgpu.h"

void world_gfx_load_image_and_wait(RECT* rect, void* data) {
    LoadImage(rect, data);
    while (DrawSync(1) != 0) { }
}
