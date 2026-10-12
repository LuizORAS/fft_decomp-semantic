#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* Drain step: once the target's result is final, a hit heals the attacker by
 * the HP damage. Against an undead target it is reversed: the attacker takes
 * the damage and the target is healed by it. */
void formula_apply_hp_absorption(void) {
    action_data_t* action;
    action_data_t* attacker;
    action_data_t* action2;
    action_data_t* action3;
    action_data_t* attacker2;
    u16 amount;

    if (g_action_target->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_UNDEAD)]
        & STATUS_BYTE_MASK(STATUS_ID_UNDEAD)) {
        action_finalize_target_current_action();
        action = g_action_target_data;
        if (action->hit != 0) {
            attacker = g_action_attacker_data;
            attacker->hp_damage = action->hp_damage;
            attacker->attack_type = ACTION_TYPE_HP_DAMAGE;
            g_action_attacker_data->hit = 1;
            action2 = g_action_target_data;
            amount = action2->hp_damage;
            action2->hp_damage = 0;
            action2->attack_type = ACTION_TYPE_HP_HEALING;
            action2->hp_healing = amount;
        }
        return;
    }
    action_finalize_target_current_action();
    action3 = g_action_target_data;
    if (action3->hit != 0) {
        attacker2 = g_action_attacker_data;
        attacker2->hp_healing = action3->hp_damage;
        attacker2->attack_type = ACTION_TYPE_HP_HEALING;
        g_action_attacker_data->hit = 1;
        g_action_target_data->attack_type = ACTION_TYPE_HP_DAMAGE;
    }
}
