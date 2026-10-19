#include "fft/battle_move.h"

/* Distortion 9 (BATTLE_DISTORTION_ANIMATION_MOVE_TO_OWN_TILE_LEVEL, queued by no SEQ file):
 * battle_move_step_unit_to_map_tile_center without moving the height until the final snap to the
 * tile. */
void battle_move_step_unit_to_map_tile_center_no_height_change(battle_unit_t* unit) {
    battle_move_step_unit_to_map_tile_center(unit, 0);
}
