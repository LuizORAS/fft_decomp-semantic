#include "fft/battle.h"

/* DeepDungeonMeshLoad game-state handler: when the Deep Dungeon map has loaded,
 * run the between-turn events. */
void battle_state_handle_deep_dungeon_mesh_load_state(void) {
    if (battle_map_update_deep_dungeon_and_animation() != 0) {
        battle_action_check_between_turn_events();
    }
}
