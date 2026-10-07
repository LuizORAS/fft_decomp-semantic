#include "fft/battle.h"

/* Formula 0x4F, Goblin Punch: the physical evade check and the hit chance MA + X with the physical
 * status modifiers, then damage equal to the caster's max HP - current HP
 * (battle_formula_43_damage_caster_missing_hp). */
void battle_formula_damage_caster_missing_hp_hit_ma_x_percent(void) {
    if (battle_formula_calculate_physical_evade() == 0) {
        battle_formula_store_ma_and_x();
        if (battle_formula_calculate_physical_status_accuracy() == 0) {
            battle_formula_43_damage_caster_missing_hp();
        }
    }
}
