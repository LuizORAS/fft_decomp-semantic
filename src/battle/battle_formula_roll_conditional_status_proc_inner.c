#include "fft/battle.h"
#include "psx/types.h"

/* When executing, a roll of 0..99 below 19 sets the added-effect flag (special
 * effect 0x0200). */
void battle_formula_roll_conditional_status_proc_inner(void) {
    if (g_battle_action_state == BATTLE_ACTION_STATE_EXECUTE && main_util_roll_pass_fail(0x64, 0x13) == 0) {
        battle_action_data_t* action = g_battle_action_target_data;
        /* The 16-bit read-modify-write sets the nullification flag. */
        action->special_effect = action->special_effect | BATTLE_ACTION_SPECIAL_EFFECT_NULLIFIED;
    }
}
