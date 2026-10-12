#include "fft/battle.h"
#include "psx/types.h"

/* Publish the panels in range to the map: a tile whose panel has range left gets
 * MAP_TILE_FLAG_ABILITY_RANGE unless it is blocked or a cross-section surface,
 * and every other tile loses the flag. Returns the number of flagged tiles. */
s32 battle_target_set_all_panels_targeted_if_targetable(void) {
    map_tile_t* tile;
    battle_target_panel_t* panel;
    s32 count;
    s32 i;

    count = 0;
    for (i = 0; i < 0x200; i++) {
        tile = &g_battle_map_tile_data[i];
        panel = &g_battle_target_panel_data[i];
        if (panel->remaining_range != 0 && !(tile->flags.value & MAP_TILE_FLAG_BLOCKED)
            && (tile->surface.type) != MAP_SURFACE_CROSS_SECTION) {
            count++;
            tile->ceiling_depth_and_marks.value |= MAP_TILE_FLAG_ABILITY_RANGE;
        } else {
            tile->ceiling_depth_and_marks.value &= ~MAP_TILE_FLAG_ABILITY_RANGE;
        }
    }
    return count;
}
