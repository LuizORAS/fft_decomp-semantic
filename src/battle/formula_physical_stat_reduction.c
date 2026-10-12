#include "fft/battle_formula.h"

/* Formula 0x2B: -PA, -MA or -Speed (X) by ability, hit (PA+Y)%; physical
 * evade. */
void formula_physical_stat_reduction(void) {
    if (formula_calculate_physical_evade() == 0) {
        formula_store_pa_and_y();
        if (formula_calculate_physical_accuracy() == 0) {
            formula_determine_reduced_stat();
        }
    }
}
