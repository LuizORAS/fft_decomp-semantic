#include "fft/battle.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Set a BATTLE draw mode from aligned texture-page coordinates in a rect.
 * Type debt (QUIRKS.md): the caller passes its window record, whose first
 * member mode0 is this DR_MODE. */
void battle_gfx_set_draw_mode_from_rect(void* mode, const RECT* rect) {
    SetDrawMode(
        mode, 0, 0, GetTPage(0, 0, rect->x & ~0x3f, rect->y & ~0xff) & 0xffff, &g_battle_menu_disabled_texture_window);
}
