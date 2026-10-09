#include "psx/types.h"

void battle_gfx_set_primitive_brightness(void* primitive, s32 brightness) {
    u8* color = primitive;

    color[4] = brightness;
    color[5] = brightness;
    color[6] = brightness;
}
