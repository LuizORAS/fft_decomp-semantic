#include "fft/battle_formula.h"

/* Formula 0x0A: status, hit F(MA+X)%; magic evade. */
void formula_hit_faith_ma_x_percent(void) {
    if (formula_calculate_magical_evade() == 0) {
        if (formula_calculate_magic_accuracy() == 0) {
            formula_apply_status_to_action();
        }
    }
}
