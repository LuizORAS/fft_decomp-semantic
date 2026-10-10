#include "fft/battle.h"

/* Absorption, then the added-effect roll. Returns the roll's result: 0 when
 * the caller should apply the status. */
s32 battle_formula_apply_elemental_absorption_and_status_proc(void) {
    battle_formula_apply_elemental_absorption();
    return battle_formula_roll_conditional_status_proc();
}
