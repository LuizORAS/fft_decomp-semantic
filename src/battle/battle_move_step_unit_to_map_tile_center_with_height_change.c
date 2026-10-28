#include "fft/battle_move.h"

/* Distortion 5 (BATTLE_DISTORTION_ANIMATION_MOVE_TO_OWN_TILE): battle_move_step_unit_to_map_tile_center, height
 * included. */
void battle_move_step_unit_to_map_tile_center_with_height_change(battle_unit_t* unit) {
    battle_move_step_unit_to_map_tile_center(unit, 1);
}
