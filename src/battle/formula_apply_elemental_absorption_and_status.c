#include "fft/battle_formula.h"
#include "psx/types.h"

/* Absorption, then the added-effect roll; when it succeeds the ability's
 * status is applied. Returns the roll's result (0 when applied). */
s32 formula_apply_elemental_absorption_and_status(void) {
    s32 result;

    formula_apply_elemental_absorption();
    result = formula_roll_conditional_status_proc();
    if (result == 0) {
        formula_apply_status();
        return 0;
    }
    return result;
}
