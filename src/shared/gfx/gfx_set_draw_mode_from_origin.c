#include "fft/gfx.h"

/* Set a draw mode from aligned texture-page coordinates from the menu text image origin. */
void gfx_set_draw_mode_from_origin(DR_MODE* mode, const point16_t* origin) {
    SetDrawMode(mode, 0, 0, GetTPage(0, 0, origin->x & ~0x3f, origin->y & ~0xff), &g_gfx_texture_window);
}
