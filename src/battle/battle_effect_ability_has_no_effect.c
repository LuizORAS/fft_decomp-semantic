#include "fft/battle.h"
#include "psx/types.h"

/* Return 1 when the ability plays no effect: its entry in g_battle_effect_ability_ids is negative (no
 * effect file). Vertical Jump8, the Charge abilities and Accumulate return 0 whatever their entry; the range
 * starts at Vertical Jump8 rather than at Charge +1. battle_action_start_strike and
 * battle_action_play_ability_effect skip the effect on 1; battle_state_handle_action_execute_state also tests
 * it. */
u32 battle_effect_ability_has_no_effect(s32 ability_id) {
    u32 result;

    if (ability_id >= ABILITY_ID_JUMP_VERTICAL_8 && ability_id < ABILITY_ID_MATH_FIRST) {
        return 0;
    }
    if (ability_id == ABILITY_ID_BASIC_SKILL_ACCUMULATE) {
        result = 0;
    } else {
        result = g_battle_effect_ability_ids[ability_id] < 0;
    }
    return result;
}
