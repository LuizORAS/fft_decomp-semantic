#include "fft/battle.h"

/* The target's Magic Defense Up: XA * 2 / 3. */
void battle_formula_apply_magic_defense_up(void) {
    /* Battle Stats 0x90 bit 0x02 is Magic Defense Up. */
    if (g_battle_action_target->support_abilities[1] & BATTLE_SUPPORT_SET_2_MAGIC_DEFENSE_UP) {
        g_battle_action_current_ability.xa = g_battle_action_current_ability.xa * 2 / 3;
    }
}
