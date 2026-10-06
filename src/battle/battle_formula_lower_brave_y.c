#include "fft/battle.h"

/* Formula 0x61, Foxbird and Chicken: the magical evade check and hit chance (MA + X with both
 * Faiths), then Brave lowered by Y (battle_formula_apply_y_brave). */
void battle_formula_lower_brave_y(void) {
    if (battle_formula_calculate_magical_evade() == 0) {
        if (battle_formula_calculate_magic_accuracy() == 0) {
            battle_formula_apply_y_brave();
        }
    }
}
