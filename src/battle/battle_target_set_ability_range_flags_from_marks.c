#include "fft/battle.h"
#include "psx/types.h"

/* Same as battle_target_set_ability_range_flags for the tiles whose panel is marked instead of in
 * range: the choices that VERTICAL_FIXED and the unit-tile flags leave. */
s32 battle_target_set_ability_range_flags_from_marks(void) {
    map_tile_t* tile;
    battle_target_panel_t* panel;
    s32 count;
    s32 i;

    count = 0;
    for (i = 0; i < 0x200; i++) {
        tile = &g_battle_map_tile_data[i];
        panel = &g_battle_target_panels[i];
        if (panel->mark != 0 && !(tile->flags.value & MAP_TILE_FLAG_BLOCKED)
            && (tile->surface.value & MAP_SURFACE_MASK) != MAP_SURFACE_CROSS_SECTION) {
            count++;
            tile->ceiling_depth_and_marks.value |= MAP_TILE_FLAG_ABILITY_RANGE;
        } else {
            tile->ceiling_depth_and_marks.value &= ~MAP_TILE_FLAG_ABILITY_RANGE;
        }
    }
    return count;
}
