#include "fft/battle_formula.h"

/* Formula 0x3D: status, hit (MA+X)%; magic evade. */
void formula_hit_ma_x_percent_magical_evade(void) {
    if (formula_calculate_magical_evade() == 0) {
        if (formula_calculate_magic_accuracy_without_faith() == 0) {
            formula_apply_status_to_action();
        }
    }
}
