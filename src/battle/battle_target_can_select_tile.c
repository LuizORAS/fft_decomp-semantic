#include "fft/battle_action.h"
#include "fft/battle_map.h"
#include "psx/types.h"

/* Remove the range from every panel, on both levels, whose tile is flagged
 * untargetable. */
void battle_target_can_select_tile(void) {
    s32 i = 0;
    battle_target_panel_t* dst = g_battle_target_panel_data;
    map_tile_t* src = g_battle_map_tile_data;
    for (; i < 0x200; i++) {
        if (src->flags.value & MAP_TILE_FLAG_UNTARGETABLE)
            dst->remaining_range = 0;
        src++;
        dst++;
    }
}
