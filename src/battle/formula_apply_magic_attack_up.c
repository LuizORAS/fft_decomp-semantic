#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* Magic Attack Up on the attacker: XA * 4/3. */
void formula_apply_magic_attack_up(void) {
    if (g_action_attacker->ability_flags.support[1] & BATTLE_SUPPORT_SET_2_MAGIC_ATTACK_UP) {
        g_action_current_ability.xa = g_action_current_ability.xa * 4 / 3;
    }
}
