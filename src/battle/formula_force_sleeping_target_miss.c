#include "fft/battle_action.h"
#include "fft/battle_formula.h"

void formula_force_sleeping_target_miss(void) {
    if ((g_action_target->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_SLEEP)] & STATUS_BYTE_MASK(STATUS_ID_SLEEP))
        != 0) {
        formula_force_attack_miss();
    }
}
