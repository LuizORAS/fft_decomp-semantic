#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* Added-effect roll of the damage formulas: returns 0 when the status or proc
 * applies. When executing that is a roll of 0..99 below 19; a preview always
 * shows it; the AI simulation never counts it. */
s32 formula_roll_conditional_status_proc(void) {
    formula_roll_conditional_status_proc_inner();
    if (g_action_state == ACTION_STATE_PREVIEW) {
        return 0;
    }
    if (g_action_target_data->special_effect & ACTION_SPECIAL_EFFECT_NULLIFIED) {
        return 0;
    }
    return 1;
}
