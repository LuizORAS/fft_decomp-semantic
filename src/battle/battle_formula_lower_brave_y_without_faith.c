#include "fft/battle_action.h"

/* Formula 0x62: -Brave (Y), hit (MA+X)%; magic evade. */
void battle_formula_lower_brave_y_without_faith(void) {
    if (battle_formula_calculate_magical_evade() == 0) {
        if (battle_formula_calculate_magic_accuracy_without_faith() == 0) {
            battle_formula_set_brave_loss_y();
        }
    }
}
