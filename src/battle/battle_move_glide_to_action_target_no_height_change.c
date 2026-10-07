#include "fft/battle.h"
#include "psx/types.h"

/* Distortion 8 (BATTLE_DISTORTION_GLIDE_TO_TARGET_LEVEL): battle_unit_move_toward_action_target at a
 * constant height. */
void battle_move_glide_to_action_target_no_height_change(battle_unit_misc_data_t* unit) {
    battle_unit_move_toward_action_target(unit, 0);
}
