#include "psx/libgpu.h"
#include "psx/types.h"

/* Set the colour of a sprite or textured quad to one grey level. */
void world_gfx_set_primitive_brightness(void* primitive, s32 brightness) {
    SPRT* sprite = primitive;

    setRGB0(sprite, brightness, brightness, brightness);
}
