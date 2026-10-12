#include "fft/battle_move.h"

/* Distortion animation 0x08: move toward the action target in equal steps without
 * changing height (battle_unit_move_toward_action_target). */
void move_accelerate_unit_to_destination_no_height_change(battle_unit_misc_data_t* unit) {
    battle_unit_move_toward_action_target(unit, 0);
}
