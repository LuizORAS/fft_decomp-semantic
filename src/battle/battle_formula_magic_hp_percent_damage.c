#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x09, Demi, Demi 2 and Lich: the magical evade check and hit chance (MA + X with both
 * Faiths, battle_formula_calculate_magic_accuracy), then damage of Y% of the target's max HP with the
 * weather and the element, the elemental absorption and the 19% status roll. */
void battle_formula_magic_hp_percent_damage(void) {
    if (battle_formula_calculate_magical_evade() == 0) {
        if (battle_formula_calculate_magic_accuracy() == 0) {
            battle_formula_calculate_hp_percent_damage();
            battle_formula_apply_weather_elemental_effects();
            battle_formula_apply_ability_element();
            if (g_battle_action_target_data->hit != 0) {
                if (battle_formula_apply_elemental_absorption_and_status_proc() == 0) {
                    battle_formula_apply_status();
                }
            }
        }
    }
}
