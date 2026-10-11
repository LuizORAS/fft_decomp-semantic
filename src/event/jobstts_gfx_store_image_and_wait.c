#include "psx/libgpu.h"

void jobstts_gfx_store_image_and_wait(RECT* rect, void* data) {
    StoreImage(rect, data);
    while (DrawSync(1) != 0) { }
}
