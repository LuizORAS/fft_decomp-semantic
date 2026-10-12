#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* Formula 0x09: damage (Y)% of max HP with element, hit F(MA+X)%; magic evade;
 * 19% status. */
void formula_magic_hp_percent_damage(void) {
    if (formula_calculate_magical_evade() == 0) {
        if (formula_calculate_magic_accuracy() == 0) {
            formula_calculate_hp_percent_damage();
            formula_apply_weather_elemental_effects();
            formula_apply_elemental();
            if (g_action_target_data->hit != 0) {
                if (formula_apply_elemental_absorption_and_status_proc() == 0) {
                    formula_apply_status();
                }
            }
        }
    }
}
