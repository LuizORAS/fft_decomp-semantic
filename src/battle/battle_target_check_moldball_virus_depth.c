#include "fft/battle_action.h"
#include "fft/battle_map.h"
#include "psx/types.h"

/* For Moldball Virus, clear the targeting panel of every tile with depth. */
void battle_target_check_moldball_virus_depth(s16 ability_id) {
    map_tile_t* tile;
    battle_target_panel_t* panel;
    s32 i;

    if (ability_id != ABILITY_ID_MONSTER_GREAT_MORBOL_MOLDBALL_VIRUS)
        return;
    i = 0;
    panel = g_battle_target_panel_data;
    tile = g_battle_map_tile_data;
    do {
        if (tile->depth_half_height.value & MAP_TILE_DEPTH_MASK) {
            panel->remaining_range = 0;
        }
        tile++;
        i += 1;
        panel++;
    } while (i < 0x200);
}
