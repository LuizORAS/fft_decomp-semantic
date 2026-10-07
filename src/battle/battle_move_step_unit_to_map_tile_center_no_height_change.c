#include "fft/battle.h"

/* Distortion 9 (BATTLE_DISTORTION_RETURN_TO_TILE_LEVEL, queued by no SEQ file):
 * battle_move_step_unit_to_map_tile_center without moving the height until the final snap to the
 * tile. */
void battle_move_step_unit_to_map_tile_center_no_height_change(battle_unit_misc_data_t* unit) {
    battle_move_step_unit_to_map_tile_center(unit, 0);
}
