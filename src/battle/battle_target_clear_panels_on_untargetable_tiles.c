#include "fft/battle.h"
#include "psx/types.h"

/* Clear the range and mark of each untargetable tile's panel, but only on the lower layer: the loop
 * stops after 256 panels (QUIRKS.md). */
void battle_target_clear_panels_on_untargetable_tiles(void) {
    s32 i;
    map_tile_t* src;
    battle_target_panel_t* dst;

    i = 0;
    dst = g_battle_target_panels;
    src = g_battle_map_tile_data;
    do {
        if (src->flags_06.value & MAP_TILE_FLAG_UNTARGETABLE) {
            dst->remaining_range = 0;
            dst->mark = 0;
        }
        src++;
        i += 1;
        dst++;
    } while (i < 0x100);
}
