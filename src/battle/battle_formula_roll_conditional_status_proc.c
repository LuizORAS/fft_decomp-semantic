#include "fft/battle_action.h"
#include "psx/types.h"

/* Added-effect roll of the damage formulas: returns 0 when the status or proc
 * applies. When executing that is a roll of 0..99 below 19; a preview always
 * shows it; the AI simulation never counts it. */
s32 battle_formula_roll_conditional_status_proc(void) {
    battle_formula_roll_conditional_status_proc_inner();
    if (g_battle_action_state == BATTLE_ACTION_STATE_PREVIEW) {
        return 0;
    }
    if (g_battle_action_target_data->special_effect & BATTLE_ACTION_SPECIAL_EFFECT_NULLIFIED) {
        return 0;
    }
    return 1;
}
