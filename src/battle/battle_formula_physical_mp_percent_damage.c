#include "fft/battle.h"

/* Formula 0x2C, Magic Break: the physical evade check and the hit chance PA + Y with the physical
 * modifiers, then MP damage of Y% of the target's max MP. */
void battle_formula_physical_mp_percent_damage(void) {
    if (battle_formula_calculate_physical_evade() == 0) {
        battle_formula_store_pa_and_y();
        if (battle_formula_calculate_physical_accuracy() == 0) {
            battle_formula_calculate_mp_percent_damage();
        }
    }
}
