#include "fft/battle_action.h"
#include "fft/battle_map.h"
#include "psx/types.h"

/* Clear MAP_TILE_FLAG_TARGETED on all 512 tiles. */
void battle_target_disable_green_panel_flags(void) {
    map_tile_t* tile;
    s32 i;

    for (i = 0; i < 0x200; i++) {
        tile = &g_battle_map_tile_data[i];
        tile->ceiling_depth_and_marks.value &= ~MAP_TILE_FLAG_TARGETED;
    }
}
