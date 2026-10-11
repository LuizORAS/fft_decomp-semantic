#include "fft/battle.h"
#include "psx/types.h"

/* Flag MAP_TILE_FLAG_ABILITY_RANGE on every tile in range that is neither blocked nor a
 * cross-section tile, clear it on the others, and return the count. */
s32 battle_target_set_ability_range_flags(void) {
    map_tile_t* tile;
    battle_target_panel_t* panel;
    s32 count;
    s32 i;

    count = 0;
    for (i = 0; i < 0x200; i++) {
        tile = &g_battle_map_tile_data[i];
        panel = &g_battle_target_panels[i];
        if (panel->remaining_range != 0 && !(tile->flags.value & MAP_TILE_FLAG_BLOCKED)
            && (tile->surface.value & MAP_SURFACE_MASK) != MAP_SURFACE_CROSS_SECTION) {
            count++;
            tile->ceiling_depth_and_marks.value |= MAP_TILE_FLAG_ABILITY_RANGE;
        } else {
            tile->ceiling_depth_and_marks.value &= ~MAP_TILE_FLAG_ABILITY_RANGE;
        }
    }
    return count;
}
