#include "fft/battle_action.h"

/* Formula 0x12: Quick, hit F(MA+X)%; no evade. */
void formula_set_quick(void) {
    formula_store_ma_and_x();
    if (formula_calculate_friendly_magic_accuracy() == 0) {
        formula_apply_quick_effect();
    }
}
