#include "fft/battle_action.h"

/* Absorption, then the added-effect roll. Returns the roll's result: 0 when
 * the caller should apply the status. */
s32 formula_apply_elemental_absorption_and_status_proc(void) {
    formula_apply_elemental_absorption();
    return formula_roll_conditional_status_proc();
}
