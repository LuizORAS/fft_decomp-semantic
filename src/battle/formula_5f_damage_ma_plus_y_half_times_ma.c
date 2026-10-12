#include "fft/battle_formula.h"

/* Formula 0x5F: damage ((MA + Y) / 2 * MA). */
void formula_5f_damage_ma_plus_y_half_times_ma(void) {
    if (formula_calculate_magical_evade() == 0) {
        formula_calculate_truth_damage();
    }
}
