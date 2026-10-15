#include "fft/battle.h"
#include "psx/types.h"

/* DEEP_DUNGEON_MESH_FINISH: step the Deep Dungeon map update; when it finishes, enter
 * AFTER_COMMAND. */
void battle_state_handle_deep_dungeon_mesh_finish_state(void) {
    if (battle_map_update_deep_dungeon_and_animation() != 0) {
        battle_state_enter_after_command();
    }
}
