#include "fft/battle.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Set a BATTLE draw mode from aligned texture-page coordinates from the menu text image origin. */
void battle_gfx_set_draw_mode_from_origin(DR_MODE* mode, const point16_t* origin) {
    SetDrawMode(
        mode, 0, 0, GetTPage(0, 0, origin->x & ~0x3f, origin->y & ~0xff), &g_battle_menu_disabled_texture_window);
}
