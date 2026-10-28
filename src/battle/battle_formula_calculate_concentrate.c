#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "fft/memory.h"

/* The attacker's Concentrate: the target's accessory, shield and class evades become 0. */
void battle_formula_calculate_concentrate(void) {
    if (g_battle_action_attacker->ability_flags.support[1] & BATTLE_SUPPORT_SET_2_CONCENTRATE) {
        main_mem_zero(&g_battle_action_current_ability.accessory_evade, 4);
    }
}
