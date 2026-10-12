#include "fft/battle_formula.h"

/* Damage MA * (MA + Y) / 2 without Faith: stores XA and YA, then runs the magical
 * damage chain of formula_calculate_truth_magical_damage. */
void formula_calculate_truth_damage(void) {
    formula_store_ma_and_ma_plus_y_divided_by_two();
    formula_calculate_truth_magical_damage();
}
