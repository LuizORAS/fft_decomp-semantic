#include "fft/battle_action.h"

/* Formula 0x0B: status, hit F(MA+X)%; no evade, and none of the target's magic
 * defences. */
void battle_formula_friendly_magic_accuracy(void) {
    battle_formula_store_ma_and_x();
    battle_formula_apply_elemental_strengthen();
    if (battle_formula_calculate_friendly_magic_accuracy() == 0) {
        battle_formula_apply_status_to_action();
    }
}
