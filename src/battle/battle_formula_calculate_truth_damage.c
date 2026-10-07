#include "fft/battle.h"

/* Damage of the Truth formula (0x1e) and of formulas 0x5e-0x60: XA = MA, YA = (MA + Y) / 2, then
 * battle_formula_calculate_magical_damage_without_faith. */
void battle_formula_calculate_truth_damage(void) {
    battle_formula_store_ma_and_ma_plus_y_divided_by_two();
    battle_formula_calculate_magical_damage_without_faith();
}
