#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x1F: damage ((MA + Y) / 2 * MA) with element, scaled by (100 -
 * caster Faith) and (100 - target Faith) over 10000, the Faith status counting
 * as 0 and Innocent as 100; magic evade; 19% status. Strikes rnd(1..X) times
 * (battle_action_init_current_ability_strike_data). */
void battle_formula_unfaith_magical_damage(void) {
    battle_action_data_t* action;

    if (battle_formula_calculate_magical_evade() == 0) {
        battle_formula_store_ma_and_ma_plus_y_divided_by_two();
        battle_formula_apply_elemental_strengthen();
        battle_formula_apply_magical_support_status_compatibility();
        if (battle_formula_calculate_elemental_xa_times_ya() == 0) {
            if (g_battle_action_attacker->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_FAITH)]
                & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FAITH)) {
                g_battle_action_current_ability.attacker_faith = 0;
            }
            if (g_battle_action_attacker->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_INNOCENT)]
                & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_INNOCENT)) {
                g_battle_action_current_ability.attacker_faith = 100;
            }
            if (g_battle_action_target->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_FAITH)]
                & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FAITH)) {
                g_battle_action_current_ability.target_faith = 0;
            }
            if (g_battle_action_target->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_INNOCENT)]
                & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_INNOCENT)) {
                g_battle_action_current_ability.target_faith = 100;
            }
            action = g_battle_action_target_data;
            action->hp_damage = action->hp_damage * (100 - g_battle_action_current_ability.target_faith)
                * (100 - g_battle_action_current_ability.attacker_faith) / 10000;
            if (battle_formula_apply_elemental_absorption_and_status_proc() == 0) {
                battle_formula_apply_status();
            }
        }
    }
}
