#include "fft/battle_move.h"

/* Distortion animation 0x04: move toward the action target in equal steps,
 * height included (battle_unit_move_toward_action_target). */
void move_accelerate_unit_to_destination_with_height_change(battle_unit_misc_data_t* unit) {
    battle_unit_move_toward_action_target(unit, 1);
}
