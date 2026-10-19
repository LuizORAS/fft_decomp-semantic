#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* The target's Defense Up: XA * 2 / 3. */
void battle_formula_apply_defense_up(void) {
    /* Battle Stats 0x90 bit 0x08 is Defense Up. */
    if (g_battle_action_target->ability_flags.support[1] & BATTLE_SUPPORT_SET_2_DEFENSE_UP) {
        g_battle_action_current_ability.xa = g_battle_action_current_ability.xa * 2 / 3;
    }
}
