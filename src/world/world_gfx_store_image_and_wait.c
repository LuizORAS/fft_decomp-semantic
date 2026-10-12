#include "fft/world.h"
#include "psx/libgpu.h"

void world_gfx_store_image_and_wait(RECT* rect, void* dest) {
    StoreImage(rect, dest);
    while (DrawSync(1) != 0) { }
}
