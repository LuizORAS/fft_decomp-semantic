#include "fft/battle_map.h"
#include "fft/battle_target.h"
#include "psx/types.h"

#include "fft/battle.h"
/* Take the untargetable tiles (MAP_TILE_FLAG_UNTARGETABLE) out of range, on both layers. */
void battle_target_clear_range_on_untargetable_tiles(void) {
    s32 i = 0;
    battle_target_panel_t* dst = g_battle_target_panels;
    map_tile_t* src = g_battle_map_tile_data;
    for (; i < MAP_TILE_SLOT_COUNT; i++) {
        if (src->flags.untargetable)
            dst->remaining_range = 0;
        src++;
        dst++;
    }
}
