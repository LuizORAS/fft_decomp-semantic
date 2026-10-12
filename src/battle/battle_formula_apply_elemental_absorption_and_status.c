#include "fft/battle_action.h"
#include "psx/types.h"

/* Absorption, then the added-effect roll; when it succeeds the ability's
 * status is applied. Returns the roll's result (0 when applied). */
s32 battle_formula_apply_elemental_absorption_and_status(void) {
    s32 result;

    battle_formula_apply_elemental_absorption();
    result = battle_formula_roll_conditional_status_proc();
    if (result == 0) {
        battle_formula_apply_status();
        return 0;
    }
    return result;
}
