#include "fft/battle.h"

/* Formula 0x24, the Geomancy abilities (Pitfall, Water Ball, Hell Ivy and nine more): the magical
 * evade check, then XA = MA and YA = (PA + Y) / 2 as magical damage without Faith
 * (battle_formula_calculate_magical_damage_without_faith). */
void battle_formula_ma_pa_half_damage(void) {
    if (battle_formula_calculate_magical_evade() == 0) {
        battle_formula_store_ma_and_pa_plus_y_divided_by_two();
        battle_formula_calculate_magical_damage_without_faith();
    }
}
