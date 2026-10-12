#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "fft/battle_script.h"
#include "psx/types.h"

/* Gil stolen: attacker Level * Speed, capped at the war funds when the target
 * is on team 0 (blue). Nothing to take is a miss. */
void formula_calculate_stolen_gil(void) {
    battle_stats_t* attacker;
    action_data_t* action;
    action_data_t* target_action;
    action_data_t* other;
    s32 amount;
    s32 available;
    s16 stolen;

    attacker = g_action_attacker;
    amount = attacker->attributes[UNIT_ATTRIBUTE_SPEED] * attacker->level;
    if (!(g_action_target->team_flags & BATTLE_TEAM_MASK)) {
        available = script_get_variable(EVENT_SCRIPT_VAR_WAR_FUNDS);
        if (available < amount) {
            amount = available;
        }
    }
    if (amount == 0) {
        formula_force_attack_miss();
        return;
    }
    action = g_action_attacker_data;
    action->hit = 1;
    other = g_action_attacker_data;
    action->gil_change = amount;
    other->attack_type = ACTION_TYPE_PSEUDO_STATUS;
    stolen = g_action_attacker_data->gil_change;
    target_action = g_action_target_data;
    target_action->attack_type = ACTION_TYPE_PSEUDO_STATUS;
    target_action->gil_change = -stolen;
}
