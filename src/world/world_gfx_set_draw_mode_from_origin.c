#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Set a WORLD draw mode from aligned texture-page coordinates from the menu text image origin. */
void world_gfx_set_draw_mode_from_origin(DR_MODE* mode, const point16_t* origin) {
    SetDrawMode(
        mode, 0, 0, GetTPage(0, 0, (s16)(origin->x & 0xffc0), (s16)(origin->y & 0xff00)), &g_world_gfx_texture_window);
}
