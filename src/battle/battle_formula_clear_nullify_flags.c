#include "fft/battle_action.h"
#include "psx/types.h"

/* Drop a pending weapon proc together with the added-effect flag (special
 * effect 0x0200). */
void battle_formula_clear_nullify_flags(void) {
    u16* pflag;
    action_data_t* action;
    u16* special_effect;
    pflag = &g_action_current_ability.weapon_proc_pending;
    if (*pflag != 0) {
        action = g_action_target_data;
        g_action_current_ability.reaction_id = 0;
        *pflag = 0;
        /* Clear the nullify bit (0x0200).  Reading the field through a
         * second pointer keeps the lhu below the two scalar global stores;
         * a direct action->special_effect load is hoisted above them
         * (MEM_IN_STRUCT_P) and the function comes out 4 bytes short. */
        special_effect = &action->special_effect;
        *special_effect &= ~ACTION_SPECIAL_EFFECT_NULLIFIED;
    }
}
