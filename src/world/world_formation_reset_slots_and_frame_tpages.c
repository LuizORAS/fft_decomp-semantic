#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Empty the eight cursor trail points (x = -1) and set the texture pages of
 * the cursor sprite and its shadow. */
void world_formation_reset_slots_and_frame_tpages(void) {
    s32 i;

    for (i = 0; i < 8; i++) {
        g_world_formation_cursor_trail[i].x = -1;
    }
    g_world_formation_cursor_shadow_sprite.source.tpage = GetTPage(0, 2, 0x3c0, 0x100);
    g_world_formation_cursor_sprite.source.tpage = GetTPage(0, 1, 0x3c0, 0x100);
}
