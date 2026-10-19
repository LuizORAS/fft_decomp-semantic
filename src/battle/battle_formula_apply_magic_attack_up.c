#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* The attacker's Magic Attack Up: XA * 4 / 3. */
void battle_formula_apply_magic_attack_up(void) {
    if (g_battle_action_attacker->ability_flags.support[1] & BATTLE_SUPPORT_SET_2_MAGIC_ATTACK_UP) {
        g_battle_action_current_ability.xa = g_battle_action_current_ability.xa * 4 / 3;
    }
}
