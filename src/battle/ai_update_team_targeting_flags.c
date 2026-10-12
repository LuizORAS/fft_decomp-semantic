#include "fft/battle_ai.h"
#include "fft/battle_unit.h"

/*
 * Update team-dependent AI targeting flags.
 *
 * Retreat support requires at least 2 eligible non-enemies, including one
 * neither Critical nor Frog. The unavailable-team-support flag propagates to
 * all cached non-enemies, not just the units admitted to those counts.
 */
void ai_update_team_targeting_flags(void) {
    ai_data_t* ai = &g_ai_data_base;
    ai_unit_decision_t* acting_decision = g_ai_data_base.acting_unit_decision;
    s32 ally_count;
    s32 enemy_count;
    s32 impaired_ally_count;
    s32 i;

    g_ai_data_base.single_enemy_remaining = 0;
    if ((acting_decision->targeting_flags_2 & AI_TARGET_TEAM_SUPPORT_UNAVAILABLE)
        || (g_ai_data_base.acting_unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DEATH_SENTENCE)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEATH_SENTENCE))) {
        g_ai_data_base.targetability.live.target_setting_flags &= ~AI_TARGET_SETTING_RETREAT_SUPPORT_AVAILABLE;
        return;
    }
    ally_count = 0;
    enemy_count = 0;
    impaired_ally_count = 0;
    for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
        if (ai_check_target_type(i)) {
            continue;
        }
        if (ai->unit_decisions[i].enemy_flag) {
            enemy_count++;
        } else {
            ally_count++;
            if (g_battle_unit_stats[i].status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_FROG)]
                & (BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FROG)
                    | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CRITICAL))) {
                impaired_ally_count++;
            }
        }
    }
    if (ally_count == impaired_ally_count || ally_count < 2) {
        if (ai->acting_unit_team) {
            for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
                if (!ai->unit_decisions[i].enemy_flag) {
                    ai->unit_decisions[i].targeting_flags_2 |= AI_TARGET_TEAM_SUPPORT_UNAVAILABLE;
                }
            }
        }
        ai->targetability.live.target_setting_flags &= ~AI_TARGET_SETTING_RETREAT_SUPPORT_AVAILABLE;
    } else {
        if (enemy_count == 1) {
            ai->single_enemy_remaining = 1;
        }
        ai->targetability.live.target_setting_flags |= AI_TARGET_SETTING_RETREAT_SUPPORT_AVAILABLE;
    }
}
