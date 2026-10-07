#include "fft/battle.h"
#include "psx/types.h"

/* Flag MAP_TILE_FLAG_ABILITY_RANGE on every tile in range that is neither blocked nor a
 * cross-section tile, clear it on the others, and return the count. */
s32 battle_target_set_ability_range_flags(void) {
    s32 count;
    s32 i;
    s32 no_tile;
    volatile map_tile_t* src;
    battle_target_panel_t* dst;

    count = 0;
    i = 0;
    no_tile = MAP_SURFACE_CROSS_SECTION;
    src = g_battle_map_tile_data;
    dst = g_battle_target_panels;
    do {
        if ((u8)dst->remaining_range != 0 && !(src->flags_06.value & MAP_TILE_FLAG_BLOCKED)
            && (src->surface.value & MAP_SURFACE_MASK) != no_tile) {
            count++;
            src->ceiling_depth_and_marks |= MAP_TILE_FLAG_ABILITY_RANGE;
        } else {
            src->ceiling_depth_and_marks &= ~MAP_TILE_FLAG_ABILITY_RANGE;
        }
        src++;
        i++;
        dst++;
    } while (i < 0x200);
    return count;
}
