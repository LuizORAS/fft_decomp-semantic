#include "fft/battle.h"
#include "psx/types.h"

/* While executing, mark the target's result PROC_TRIGGERED on a 0-99 roll below 19: the 19% chance
 * of a weapon's or an ability's added effect. */
void battle_formula_roll_conditional_status_proc_inner(void) {
    if (g_battle_action_state == BATTLE_ACTION_STATE_EXECUTE && main_util_roll_pass_fail(0x64, 0x13) == 0) {
        battle_action_data_t* action = g_battle_action_target_data;
        /* The halfword read-modify-write sets the proc flag. */
        action->special_effect = action->special_effect | BATTLE_ACTION_SPECIAL_EFFECT_PROC_TRIGGERED;
    }
}
