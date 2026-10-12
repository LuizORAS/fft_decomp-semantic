#include "fft/battle_action.h"

/* Concentrate on the attacker: the target's four evades become 0. */
void battle_formula_calculate_concentrate(void) {
    if (g_battle_action_attacker->support_abilities[1] & BATTLE_SUPPORT_SET_2_CONCENTRATE) {
        main_util_clear_byte_data(&g_battle_action_current_ability.accessory_evade, 4);
    }
}
