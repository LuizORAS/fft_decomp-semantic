#include "fft/battle.h"

/* Formula 0x12, Quick: no evade check; hit chance MA + X with both Faiths, then Quick
 * (battle_formula_apply_quick_effect). */
void battle_formula_set_quick(void) {
    battle_formula_store_ma_and_x();
    if (battle_formula_calculate_friendly_magic_accuracy() == 0) {
        battle_formula_apply_quick_effect();
    }
}
