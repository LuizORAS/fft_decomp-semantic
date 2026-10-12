#include "fft/battle_action.h"

/* Formula 0x31: damage ((PA + Y) / 2 * PA) with element; physical evade; 19%
 * status. */
void formula_damage_pa_plus_y_half_times_pa(void) {
    if (formula_calculate_physical_evade() == 0) {
        formula_store_pa_and_pa_plus_y_divided_by_two();
        formula_apply_elemental_strengthen();
        formula_apply_attack_up_and_martial_arts();
        formula_apply_physical_status_support_compatibility();
        formula_calculate_critical_hit();
        formula_store_xa_times_ya_damage();
        formula_apply_weather_elemental_effects();
        formula_apply_elemental();
        if (g_action_target_data->hit != 0) {
            formula_apply_elemental_absorption();
            if (formula_roll_conditional_status_proc() == 0) {
                formula_apply_status();
            }
        }
    }
}
