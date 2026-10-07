#include "fft/battle.h"
#include "psx/types.h"

/* DEEP_DUNGEON_MESH_LOAD: step the Deep Dungeon map update; when it finishes, check the
 * between-turn events. */
void battle_state_handle_deep_dungeon_mesh_load_state(void) {
    if (battle_map_update_deep_dungeon_and_animation() != 0) {
        battle_turn_advance();
    }
}
