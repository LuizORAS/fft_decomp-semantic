#include "fft/battle_move.h"

/* Distortion 4 (BATTLE_DISTORTION_ANIMATION_MOVE_TO_TARGET): battle_unit_move_toward_action_target, height
 * included. */
void battle_move_glide_to_action_target_with_height_change(battle_unit_t* unit) {
    battle_unit_move_toward_action_target(unit, 1);
}
