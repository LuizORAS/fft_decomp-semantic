#include "fft/gfx.h"

/* Set a draw mode from the aligned texture-page coordinates of an origin: BATTLE's and WORLD's
 * window builders pass their menu text image origin and the record's DR_MODE. */
void gfx_set_draw_mode_from_origin(DR_MODE* mode, const point16_t* origin) {
    SetDrawMode(mode, 0, 0, GetTPage(0, 0, origin->x & ~0x3f, origin->y & ~0xff), &g_menu_disabled_texture_window);
}
