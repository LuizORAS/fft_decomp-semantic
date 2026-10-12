#include "fft/battle_move.h"

/* Distortion animation 0x05: move to the centre of the unit's map tile in equal
 * steps, height included (battle_move_step_unit_to_map_tile_center). */
void battle_move_step_unit_to_map_tile_center_with_height_change(battle_unit_misc_data_t* unit) {
    battle_move_step_unit_to_map_tile_center(unit, 1);
}
