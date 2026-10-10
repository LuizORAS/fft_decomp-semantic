#include "fft/battle.h"

/* Distortion animation 0x09: move to the centre of the unit's map tile without
 * changing height. */
void battle_move_step_unit_to_map_tile_center_no_height_change(battle_unit_misc_data_t* unit) {
    battle_move_step_unit_to_map_tile_center(unit, 0);
}
