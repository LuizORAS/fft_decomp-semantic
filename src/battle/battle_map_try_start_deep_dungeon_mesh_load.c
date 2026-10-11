#include "fft/battle.h"

/* Between turns: when the crystal count calls for a map reload
 * (battle_map_check_deep_dungeon_crystal_and_init_state), enter the Deep Dungeon
 * mesh-load state at animation speed 1. Returns 1 when the state changed. */
s32 battle_map_try_start_deep_dungeon_mesh_load(void) {
    if (battle_map_check_deep_dungeon_crystal_and_init_state() != 0) {
        g_battle_game_state = BATTLE_GAME_STATE_DEEP_DUNGEON_MESH_LOAD;
        g_main_system_vsync_interval = 1;
        return 1;
    }

    return 0;
}
