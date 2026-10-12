#include "fft/battle_action.h"

void battle_formula_force_sleeping_target_miss(void) {
    if ((g_action_target->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_SLEEP)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_SLEEP))
        != 0) {
        battle_formula_force_attack_miss();
    }
}
