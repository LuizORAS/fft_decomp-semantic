#include "fft/battle.h"

/* battle_formula_apply_elemental_absorption, then returns the status roll
 * (battle_formula_roll_conditional_status_proc): 0 when the caller applies the ability's status. */
s32 battle_formula_apply_elemental_absorption_and_status_proc(void) {
    battle_formula_apply_elemental_absorption();
    return battle_formula_roll_conditional_status_proc();
}
