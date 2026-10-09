#include "fft/battle.h"

void battle_formula_apply_attacker_berserk_frog(void) {
    if (g_battle_action_attacker->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_BERSERK)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_BERSERK)) {
        g_battle_action_current_ability.xa = g_battle_action_current_ability.xa * 3 / 2;
    }
    if (g_battle_action_attacker->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_FROG)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FROG)) {
        g_battle_action_current_ability.xa = 1;
    }
}
