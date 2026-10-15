#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x53, Hurricane and Triple Bracelet: the magical evade check and the hit chance MA + X
 * without Faith, then damage of Y% of the target's max HP with the weather and the element, the
 * absorption and the 19% status roll. */
void battle_formula_damage_y_percent_hit_ma_x_percent(void) {
    if (battle_formula_calculate_magical_evade() != 0) {
        return;
    }
    if (battle_formula_calculate_magic_accuracy_without_faith() != 0) {
        return;
    }
    battle_formula_calculate_hp_percent_damage();
    battle_formula_apply_weather_elemental_effects();
    battle_formula_apply_ability_element();
    if (g_battle_action_target_data->hit != 0 && battle_formula_apply_elemental_absorption_and_status_proc() == 0) {
        battle_formula_apply_status();
    }
}
