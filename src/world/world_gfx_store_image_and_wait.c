#include "psx/libgpu.h"
#include "psx/types.h"

void world_gfx_store_image_and_wait(RECT* rect, u32* dest) {
    StoreImage(rect, dest);
    while (DrawSync(1) != 0) { }
}
