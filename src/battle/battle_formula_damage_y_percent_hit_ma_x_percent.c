#include "fft/battle.h"

/* Formula 0x53: damage (Y)% of max HP, hit (MA+X)%. */
void battle_formula_damage_y_percent_hit_ma_x_percent(void) {
    if (battle_formula_calculate_magical_evade() != 0) {
        return;
    }
    if (battle_formula_calculate_magic_accuracy_without_faith() != 0) {
        return;
    }
    battle_formula_calculate_hp_percent_damage();
    battle_formula_apply_weather_elemental_effects();
    battle_formula_apply_elemental();
    if (g_battle_action_target_data->hit != 0 && battle_formula_apply_elemental_absorption_and_status_proc() == 0) {
        battle_formula_apply_status();
    }
}
