#include "fft/battle.h"
#include "psx/types.h"

/* Publish the marked panels to the map: a tile whose panel mark is set gets
 * MAP_TILE_FLAG_ABILITY_RANGE unless it is blocked or a cross-section surface,
 * and every other tile loses the flag. Returns the number of flagged tiles.
 *
 * battle_target_set_ability_panels ends with it for abilities that have the
 * vertical-fixed or a unit-panels flag; the others end with
 * battle_target_set_all_panels_targeted_if_targetable, which tests the panel's
 * range instead of its mark. */
s32 battle_target_clear_selection_state_of_all_panels(void) {
    map_tile_t* tile;
    targeting_panel_entry_t* panel;
    s32 count;
    s32 i;

    count = 0;
    for (i = 0; i < 0x200; i++) {
        tile = &g_battle_map_tile_data[i];
        panel = &g_battle_target_panel_data[i];
        if (panel->b != 0 && !(tile->flags.value & MAP_TILE_FLAG_BLOCKED)
            && (tile->surface.value & MAP_SURFACE_MASK) != MAP_SURFACE_CROSS_SECTION) {
            count++;
            tile->ceiling_depth_and_marks.value |= MAP_TILE_FLAG_ABILITY_RANGE;
        } else {
            tile->ceiling_depth_and_marks.value &= ~MAP_TILE_FLAG_ABILITY_RANGE;
        }
    }
    return count;
}
