#include "fft/battle.h"
#include "psx/types.h"

/* Cancel a pending weapon spell: clear weapon_spell_pending and reaction_id, and the target's
 * NULLIFIED special effect, so the weapon's hit stands. battle_action_finalize_target_current_action
 * calls it for an invalid target and for a lethal hit. */
void battle_formula_cancel_weapon_spell(void) {
    u16* pflag;
    battle_action_data_t* action;
    u16* special_effect;
    pflag = &g_current_ability.weapon_spell_pending;
    if (*pflag != 0) {
        action = g_battle_action_target_data;
        g_current_ability.reaction_id = 0;
        *pflag = 0;
        /* Clear the nullify bit (0x0200).  Reading the field through a
         * second pointer keeps the lhu below the two scalar global stores;
         * a direct action->special_effect load is hoisted above them
         * (MEM_IN_STRUCT_P) and the function comes out 4 bytes short. */
        special_effect = &action->special_effect;
        *special_effect &= ~BATTLE_ACTION_SPECIAL_EFFECT_NULLIFIED;
    }
}
