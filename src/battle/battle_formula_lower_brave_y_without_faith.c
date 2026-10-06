#include "fft/battle.h"

/* Formula 0x62, Look of Fright: formula 0x61 without Faith in the hit chance. */
void battle_formula_lower_brave_y_without_faith(void) {
    if (battle_formula_calculate_magical_evade() == 0) {
        if (battle_formula_calculate_magic_accuracy_without_faith() == 0) {
            battle_formula_apply_y_brave();
        }
    }
}
