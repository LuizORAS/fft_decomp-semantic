#include "fft/battle.h"
#include "psx/types.h"

/* Drain MP: the stored amount becomes MP damage on the target; once its result is finalized
 * (battle_action_finalize_target_current_action), a hit target loses that MP and the attacker
 * restores the same. Unlike battle_formula_apply_hp_absorption, an undead target does not reverse
 * it. */
void battle_formula_apply_mp_absorption(void) {
    u16 mp_amount;
    battle_action_data_t* action;
    battle_action_data_t* action_after;

    action = g_battle_action_target_data;
    mp_amount = action->hp_damage;
    action->hp_damage = 0;
    action->attack_type = BATTLE_ACTION_TYPE_MP_DAMAGE;
    action->mp_damage = mp_amount;
    battle_action_finalize_target_current_action();
    action_after = g_battle_action_target_data;
    if (action_after->hit != 0) {
        battle_action_data_t* reaction = g_battle_action_attacker_data;
        reaction->mp_healing = action_after->mp_damage;
        reaction->attack_type = BATTLE_ACTION_TYPE_MP_HEALING;
        g_battle_action_attacker_data->hit = 1;
    }
}
