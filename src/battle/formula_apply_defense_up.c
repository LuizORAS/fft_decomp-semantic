#include "fft/battle_action.h"

/* Defense Up on the target: XA * 2/3. */
void formula_apply_defense_up(void) {
    /* Battle Stats 0x90 bit 0x08 is Defense Up. */
    if (g_action_target->support_abilities[1] & BATTLE_SUPPORT_SET_2_DEFENSE_UP) {
        g_action_current_ability.xa = g_action_current_ability.xa * 2 / 3;
    }
}
