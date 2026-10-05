#include "fft/battle.h"
#include "psx/types.h"

/* Distortion 4 (BATTLE_DISTORTION_GLIDE_TO_TARGET): battle_unit_move_toward_action_target, height
 * included. */
void battle_move_glide_to_action_target_with_height_change(battle_unit_misc_data_t* unit) {
    battle_unit_move_toward_action_target(unit, 1);
}
