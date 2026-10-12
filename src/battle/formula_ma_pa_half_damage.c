#include "fft/battle_formula.h"

/* Formula 0x24: damage ((PA + Y) / 2 * MA) with element; magic evade; 19%
 * status. */
void formula_ma_pa_half_damage(void) {
    if (formula_calculate_magical_evade() == 0) {
        formula_store_ma_and_pa_plus_y_divided_by_two();
        formula_calculate_truth_magical_damage();
    }
}
