#include "psx/libgpu.h"

void bunit_gfx_load_image_and_wait(RECT* rect, void* data) {
    LoadImage(rect, data);
    while (DrawSync(1) != 0) { }
}
