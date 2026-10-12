#include "fft/battle_formula.h"

/* Formula 0x61: -Brave (Y), hit F(MA+X)%; magic evade. */
void formula_lower_brave_y(void) {
    if (formula_calculate_magical_evade() == 0) {
        if (formula_calculate_magic_accuracy() == 0) {
            formula_set_brave_loss_y();
        }
    }
}
