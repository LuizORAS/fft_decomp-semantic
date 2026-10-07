#include "fft/battle.h"

/* Distortion 5 (BATTLE_DISTORTION_RETURN_TO_TILE): battle_move_step_unit_to_map_tile_center, height
 * included. */
void battle_move_step_unit_to_map_tile_center_with_height_change(battle_unit_misc_data_t* unit) {
    battle_move_step_unit_to_map_tile_center(unit, 1);
}
