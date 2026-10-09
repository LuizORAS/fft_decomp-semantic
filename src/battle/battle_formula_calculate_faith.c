#include "fft/battle.h"

void battle_formula_calculate_faith(void) {
    u8* act;

    if (g_battle_action_attacker->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_FAITH)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FAITH)) {
        g_current_ability.attacker_faith = 100;
    }
    if (g_battle_action_attacker->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_INNOCENT)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_INNOCENT)) {
        g_current_ability.attacker_faith = 0;
    }
    if (g_battle_action_target->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_FAITH)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FAITH)) {
        g_current_ability.target_faith = 100;
    }
    if (g_battle_action_target->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_INNOCENT)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_INNOCENT)) {
        g_current_ability.target_faith = 0;
    }

    act = g_battle_action_target_data;
    *(s16*)(act + 4) = *(s16*)(act + 4) * g_current_ability.target_faith * g_current_ability.attacker_faith / 10000;
}
