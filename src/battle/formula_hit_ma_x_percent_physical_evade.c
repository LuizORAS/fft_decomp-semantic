#include "fft/battle_formula.h"

/* Formula 0x50: status, hit (MA+X)%; physical evade. */
void formula_hit_ma_x_percent_physical_evade(void) {
    if (formula_calculate_physical_evade() == 0) {
        formula_store_ma_and_x();
        if (formula_calculate_physical_status_accuracy() == 0) {
            formula_apply_status_to_action();
        }
    }
}
