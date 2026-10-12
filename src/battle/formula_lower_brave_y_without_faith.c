#include "fft/battle_action.h"

/* Formula 0x62: -Brave (Y), hit (MA+X)%; magic evade. */
void formula_lower_brave_y_without_faith(void) {
    if (formula_calculate_magical_evade() == 0) {
        if (formula_calculate_magic_accuracy_without_faith() == 0) {
            formula_set_brave_loss_y();
        }
    }
}
