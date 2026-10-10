#include "fft/battle.h"

/* Formula 0x12: Quick, hit F(MA+X)%; no evade. */
void battle_formula_set_quick(void) {
    battle_formula_store_ma_and_x();
    if (battle_formula_calculate_friendly_magic_accuracy() == 0) {
        battle_formula_apply_quick_effect();
    }
}
