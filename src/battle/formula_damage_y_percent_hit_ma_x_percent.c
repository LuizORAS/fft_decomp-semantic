#include "fft/battle_action.h"

/* Formula 0x53: damage (Y)% of max HP, hit (MA+X)%. */
void formula_damage_y_percent_hit_ma_x_percent(void) {
    if (formula_calculate_magical_evade() != 0) {
        return;
    }
    if (formula_calculate_magic_accuracy_without_faith() != 0) {
        return;
    }
    formula_calculate_hp_percent_damage();
    formula_apply_weather_elemental_effects();
    formula_apply_elemental();
    if (g_action_target_data->hit != 0 && formula_apply_elemental_absorption_and_status_proc() == 0) {
        formula_apply_status();
    }
}
