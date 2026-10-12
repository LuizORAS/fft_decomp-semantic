#include "fft/battle_action.h"

/* Formula 0x2D: damage (PA * (WP + Y)), 100% status. */
void formula_damage_pa_times_wp_plus_y_status(void) {
    if (formula_calculate_physical_evade() == 0) {
        formula_store_pa_and_weapon_power_plus_y();
        formula_apply_weapon_element_strengthen();
        formula_calculate_physical_damage();
        formula_modify_elemental_damage();
        if (g_action_target_data->hit != 0) {
            formula_apply_elemental_absorption();
            formula_apply_status();
        }
    }
}
