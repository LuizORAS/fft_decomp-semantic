#include "fft/battle.h"
#include "psx/types.h"

/* battle_formula_apply_elemental_absorption, then the status roll
 * (battle_formula_roll_conditional_status_proc); when the roll returns 0 it applies the ability's
 * status (battle_formula_apply_status). Returns the roll's result. */
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
