#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "fft/memory.h"

/* Concentrate on the attacker: the target's four evades become 0. */
void formula_calculate_concentrate(void) {
    if (g_action_attacker->ability_flags.support[1] & BATTLE_SUPPORT_SET_2_CONCENTRATE) {
        mem_zero(&g_action_current_ability.accessory_evade, 4);
    }
}
