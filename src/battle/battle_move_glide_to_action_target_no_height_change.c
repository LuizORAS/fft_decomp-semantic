#include "fft/battle_move.h"

/* Distortion 8 (BATTLE_DISTORTION_ANIMATION_MOVE_TO_TARGET_LEVEL): battle_unit_move_toward_action_target at a
 * constant height. */
void battle_move_glide_to_action_target_no_height_change(battle_unit_t* unit) {
    battle_unit_move_toward_action_target(unit, 0);
}
