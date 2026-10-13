#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "fft/unit.h"

/* Formula 0x1F: damage ((MA + Y) / 2 * MA) with element, scaled by (100 -
 * caster Faith) and (100 - target Faith) over 10000, the Faith status counting
 * as 0 and Innocent as 100; magic evade; 19% status. Strikes rnd(1..X) times
 * (action_init_current_ability_strike_data). */
void formula_unfaith_magical_damage(void) {
    action_data_t* action;

    if (formula_calculate_magical_evade() != 0) {
        return;
    }
    formula_store_ma_and_ma_plus_y_divided_by_two();
    formula_apply_elemental_strengthen();
    formula_apply_magical_support_status_compatibility();
    if (formula_calculate_elemental_xa_times_ya() != 0) {
        return;
    }
    if (g_action_attacker->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_FAITH)]
        & STATUS_BYTE_MASK(STATUS_ID_FAITH)) {
        g_action_current_ability.attacker_faith = 0;
    }
    if (g_action_attacker->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_INNOCENT)]
        & STATUS_BYTE_MASK(STATUS_ID_INNOCENT)) {
        g_action_current_ability.attacker_faith = 100;
    }
    if (g_action_target->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_FAITH)] & STATUS_BYTE_MASK(STATUS_ID_FAITH)) {
        g_action_current_ability.target_faith = 0;
    }
    if (g_action_target->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_INNOCENT)]
        & STATUS_BYTE_MASK(STATUS_ID_INNOCENT)) {
        g_action_current_ability.target_faith = 100;
    }
    action = g_action_target_data;
    action->hp_damage = action->hp_damage * (100 - g_action_current_ability.target_faith)
        * (100 - g_action_current_ability.attacker_faith) / 10000;
    if (formula_apply_elemental_absorption_and_status_proc() == 0) {
        formula_apply_status();
    }
}
