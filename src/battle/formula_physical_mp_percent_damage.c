#include "fft/battle_formula.h"

/* Formula 0x2C: MP damage (Y)% of max MP, hit (PA+Y)%; physical evade. */
void formula_physical_mp_percent_damage(void) {
    if (formula_calculate_physical_evade() == 0) {
        formula_store_pa_and_y();
        if (formula_calculate_physical_accuracy() == 0) {
            formula_calculate_mp_percent_damage();
        }
    }
}
