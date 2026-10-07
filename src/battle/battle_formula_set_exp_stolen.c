#include "fft/battle.h"

/* volatile views: the target reloads these globals at every use. */
extern battle_stats_t* volatile g_battle_action_attacker;
extern battle_stats_t* volatile g_battle_action_target;

/* Steal EXP: take Speed + Y EXP, at most 100 and at most what the target has; the attacker's result
 * gains it and the target's loses it (bit 0x80 of exp_change, battle_action_apply_exp_change). With
 * nothing to take the action is a forced failure. */
void battle_formula_set_exp_stolen(void) {
    u8 amount;
    u8 available_exp;

    amount = g_battle_action_attacker->attributes[UNIT_ATTRIBUTE_SPEED] + g_current_ability.range_data.y;
    if (amount > 100) {
        amount = 100;
    }
    available_exp = g_battle_action_target->experience;
    if (available_exp < amount) {
        amount = available_exp;
    }
    if (amount == 0) {
        battle_formula_force_attack_miss();
        return;
    }
    g_battle_action_attacker_data->exp_change = amount;
    g_battle_action_attacker_data->hit = 1;
    g_battle_action_attacker_data->attack_type = BATTLE_ACTION_TYPE_PSEUDO_STATUS;
    g_battle_action_target_data->exp_change = amount + 0x80;
    g_battle_action_target_data->attack_type = BATTLE_ACTION_TYPE_PSEUDO_STATUS;
}
