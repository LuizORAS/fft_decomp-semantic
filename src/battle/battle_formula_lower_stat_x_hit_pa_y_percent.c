#include "fft/battle.h"

/* Formula 0x2B, Speed Break, Power Break and Mind Break: the physical evade check and the hit chance
 * PA + Y with the physical modifiers (battle_formula_calculate_physical_accuracy), then Speed, PA or
 * MA lowered by X (battle_formula_determine_reduced_stat). */
void battle_formula_lower_stat_x_hit_pa_y_percent(void) {
    if (battle_formula_calculate_physical_evade() == 0) {
        battle_formula_store_pa_and_y();
        if (battle_formula_calculate_physical_accuracy() == 0) {
            battle_formula_determine_reduced_stat();
        }
    }
}
