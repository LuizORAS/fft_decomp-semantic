#include "fft/battle_action.h"

/*
 * Record the stolen EXP on both action results.
 *
 * The target's loss is encoded by adding 0x80 to the amount.
 */
void formula_set_exp_stolen(void) {
    u8 amount;
    u8 available_exp;

    amount = g_action_attacker->attributes[UNIT_ATTRIBUTE_SPEED] + g_action_current_ability.range_data.y;
    if (amount > 100) {
        amount = 100;
    }
    available_exp = g_action_target->experience;
    if (available_exp < amount) {
        amount = available_exp;
    }
    if (amount == 0) {
        formula_force_attack_miss();
        return;
    }
    g_action_attacker_data->exp_change = amount;
    g_action_attacker_data->hit = 1;
    g_action_attacker_data->attack_type = ACTION_TYPE_PSEUDO_STATUS;
    g_action_target_data->exp_change = amount + 0x80;
    g_action_target_data->attack_type = ACTION_TYPE_PSEUDO_STATUS;
}
