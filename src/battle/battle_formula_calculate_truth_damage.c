#include "fft/battle.h"

/* Damage MA * (MA + Y) / 2 without Faith: stores XA and YA, then runs the magical
 * damage chain of battle_formula_calculate_truth_magical_damage. */
void battle_formula_calculate_truth_damage(void) {
    battle_formula_store_ma_and_ma_plus_y_divided_by_two();
    battle_formula_calculate_truth_magical_damage();
}
