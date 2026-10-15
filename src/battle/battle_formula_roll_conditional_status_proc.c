#include "fft/battle.h"
#include "psx/types.h"

/* The 19% roll for an added effect (battle_formula_roll_conditional_status_proc_inner): returns 0
 * when the caller adds the ability's status or casts the weapon's spell, that is always in a preview
 * (which shows the effect) and, while executing, when the roll set PROC_TRIGGERED. Returns 1
 * otherwise, and always in an AI simulation, which does not count the effect. */
s32 battle_formula_roll_conditional_status_proc(void) {
    battle_formula_roll_conditional_status_proc_inner();
    if (g_battle_action_state == BATTLE_ACTION_STATE_PREVIEW) {
        return 0;
    }
    if (g_battle_action_target_data->special_effect & BATTLE_ACTION_SPECIAL_EFFECT_PROC_TRIGGERED) {
        return 0;
    }
    return 1;
}
