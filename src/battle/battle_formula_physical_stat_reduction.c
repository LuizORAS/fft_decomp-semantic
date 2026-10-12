#include "fft/battle_action.h"

/* Formula 0x2B: -PA, -MA or -Speed (X) by ability, hit (PA+Y)%; physical
 * evade. */
void battle_formula_physical_stat_reduction(void) {
    if (battle_formula_calculate_physical_evade() == 0) {
        battle_formula_store_pa_and_y();
        if (battle_formula_calculate_physical_accuracy() == 0) {
            battle_formula_determine_reduced_stat();
        }
    }
}
