#include "fft/battle_action.h"

/* Formula 0x61: -Brave (Y), hit F(MA+X)%; magic evade. */
void battle_formula_lower_brave_y(void) {
    if (battle_formula_calculate_magical_evade() == 0) {
        if (battle_formula_calculate_magic_accuracy() == 0) {
            battle_formula_set_brave_loss_y();
        }
    }
}
