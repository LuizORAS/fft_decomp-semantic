#include "fft/battle.h"
#include "psx/types.h"

s32 battle_target_clear_selection_state_of_all_panels(void) {
    map_tile_t* tile;
    targeting_panel_entry_t* panel;
    s32 count;
    s32 i;

    count = 0;
    for (i = 0; i < 0x200; i++) {
        tile = &g_battle_map_tile_data[i];
        panel = &g_battle_target_panel_data[i];
        if ((u8)panel->b != 0 && !(tile->flags.value & MAP_TILE_FLAG_BLOCKED)
            && (tile->surface.value & MAP_SURFACE_MASK) != MAP_SURFACE_CROSS_SECTION) {
            count++;
            tile->ceiling_depth_and_marks.value |= MAP_TILE_FLAG_ABILITY_RANGE;
        } else {
            tile->ceiling_depth_and_marks.value &= ~MAP_TILE_FLAG_ABILITY_RANGE;
        }
    }
    return count;
}
