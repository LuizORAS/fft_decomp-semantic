#include "fft/battle.h"

/* The target's Defense Up: XA * 2 / 3. */
void battle_formula_apply_defense_up(void) {
    s16* xa;
    /* Battle Stats 0x90 bit 0x08 is Defense Up. */
    if (g_battle_action_target->support_abilities[1] & BATTLE_SUPPORT_SET_2_DEFENSE_UP) {
        xa = (s16*)&g_current_ability.xa;
        *xa = (*xa * 2) / 3;
    }
}
