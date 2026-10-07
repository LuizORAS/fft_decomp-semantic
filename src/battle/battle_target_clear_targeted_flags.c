#include "fft/battle.h"
#include "psx/types.h"

/* Clear MAP_TILE_FLAG_TARGETED on every tile. */
void battle_target_clear_targeted_flags(void) {
    s32 i;
    volatile map_tile_t* tile;
    i = 0;
    tile = g_battle_map_tile_data;
    do {
        i++;
        tile->ceiling_depth_and_marks &= ~MAP_TILE_FLAG_TARGETED;
        tile++;
    } while (i < 0x200);
}
