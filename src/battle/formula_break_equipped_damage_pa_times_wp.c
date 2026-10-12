#include "fft/battle_action.h"

/* Formula 0x2E: break the equipment the ability names and damage (PA * WP)
 * with the weapon's element; physical evade. Fails with nothing in that slot
 * or against Maintenance. */
void formula_break_equipped_damage_pa_times_wp(void) {
    action_data_t* action;

    if (formula_calculate_physical_evade() != 0) {
        return;
    }
    if (formula_apply_steal_break_might_sword_hardcoding() != 0) {
        formula_nullify_action();
        g_action_target_data->miss_type = ACTION_MISS_TYPE_FORCED_FAILURE;
        return;
    }
    formula_apply_maintenance();
    action = g_action_target_data;
    if (action->hit == 0) {
        return;
    }
    /* The target writes the combined special-effect field as one 16-bit value. */
    action->special_effect = ACTION_SPECIAL_EFFECT_BREAK_EQUIPMENT;
    formula_store_pa_and_weapon_power();
    formula_apply_weapon_element_strengthen();
    formula_calculate_physical_damage();
    formula_modify_elemental_damage();
    if (g_action_target_data->hit == 0) {
        return;
    }
    formula_apply_elemental_absorption();
}
