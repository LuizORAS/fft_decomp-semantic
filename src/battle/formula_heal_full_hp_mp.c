#include "fft/battle_formula.h"

/* Formula 0x4A: heal 100% HP and 100% MP. */
void formula_heal_full_hp_mp(void) {
    formula_apply_full_hp_mp_heal();
    formula_apply_undead_reversal();
}
