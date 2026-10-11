#include "fft/world.h"
#include "psx/libgpu.h"

/* Set the colour in a GPU primitive's common header to one grey level. */
void world_gfx_set_primitive_brightness(P_TAG* primitive, s32 brightness) {
    setRGB0(primitive, brightness, brightness, brightness);
}
