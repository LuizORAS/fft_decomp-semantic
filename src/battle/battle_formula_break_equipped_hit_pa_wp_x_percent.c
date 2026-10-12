#include "fft/battle_action.h"

/* Formula 0x25: break the equipment the ability names, hit (PA+WP+Y)%;
 * physical evade; Maintenance blocks it. With nothing in that slot the action
 * becomes a plain attack. */
void battle_formula_break_equipped_hit_pa_wp_x_percent(void) {
    action_data_t* action;

    if (battle_formula_apply_steal_break_might_sword_hardcoding() != 0) {
        g_action_current_ability.defaulted_to_attack = 1;
        action_switch_ability_to_default_attack();
        return;
    }
    g_action_current_ability.defaulted_to_attack = 0;
    if (battle_formula_calculate_physical_evade() != 0) {
        return;
    }
    battle_formula_store_pa_and_weapon_power_plus_y();
    if (battle_formula_calculate_physical_accuracy() != 0) {
        /* The target writes the combined special-effect field as one 16-bit value. */
        g_action_target_data->special_effect = 0;
        return;
    }
    battle_formula_apply_maintenance();
    action = g_action_target_data;
    if (action->hit != 0) {
        action->special_effect = ACTION_SPECIAL_EFFECT_BREAK_EQUIPMENT;
    }
}
