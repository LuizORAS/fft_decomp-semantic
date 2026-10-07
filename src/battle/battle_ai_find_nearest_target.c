#include "fft/battle.h"

/* Return the nearest unit (Manhattan distance; the first slot on ties) that the mode admits, or the
 * acting unit when none does. Every mode skips untargetable and petrified units, and every mode but 4
 * the dead: 0 skips Blood Suck; 1 takes targetable enemies that act before the acting unit and are not
 * disabled by then, unless they stand next to it more than 2 levels lower; 2 takes any enemy; 3 any
 * unit; 4 takes targetable enemies worth targeting (BATTLE_AI_DECISION_WORTH_TARGETING) that are not
 * dead without Reraise, and skips Blood Suck; 5 takes allied healers, and crystals. */
s32 battle_ai_find_nearest_target(battle_ai_nearest_target_mode_e mode) {
    s32 nearest_distance = 0x7fffffff;
    s32 nearest_id = g_battle_ai_acting_unit_id;
    s32 i;
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    battle_stats_t* unit;
    battle_ai_unit_decision_t* decision;
    s32 distance;

    for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
        unit = &g_battle_unit_stats[i];
        decision = &ai->unit_decisions[i];
        /* Crystals always qualify as healer-mode destinations. */
        if (mode != BATTLE_AI_NEAREST_HEALER_OR_CRYSTAL
            || !(unit->status_sets.current[0] & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CRYSTAL))) {
            if (battle_ai_can_unit_be_targeted_cryst_trea_mount_trans(i))
                continue;
            if (unit->status_sets.current[1] & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_PETRIFY))
                continue;
            if (mode != BATTLE_AI_NEAREST_ELIGIBLE_ENEMY
                && (unit->status_sets.current[0] & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEAD)))
                continue;
            switch (mode) {
            case BATTLE_AI_NEAREST_HEALER_OR_CRYSTAL:
                if (decision->enemy_flag || !(decision->targeting_flags_1 & BATTLE_AI_TARGETING_FLAG_1_HEALER))
                    continue;
                break;
            case BATTLE_AI_NEAREST_IMMINENT_ENEMY:
                if (!ai->targetability.live.unit_targetable[i])
                    continue;
                if (battle_ai_should_exclude_from_imminent_enemies(i))
                    continue;
                if (battle_ai_calculate_distance_between_units(ai->acting_unit, unit) < 2
                    && battle_ai_calculate_height_difference_between_units(&ai->acting_unit_coords, unit) < -2)
                    continue;
                break;
            case BATTLE_AI_NEAREST_ENEMY:
                if (!decision->enemy_flag)
                    continue;
                break;
            case BATTLE_AI_NEAREST_ELIGIBLE_ENEMY:
                if (!decision->enemy_flag || (decision->targeting_flags_2 & BATTLE_AI_TARGET_DEAD_WITHOUT_RERAISE))
                    continue;
                if (!ai->targetability.live.unit_targetable[i]
                    || !(decision->flags & BATTLE_AI_DECISION_WORTH_TARGETING))
                    continue;
            case BATTLE_AI_NEAREST_WITHOUT_BLOOD_SUCK:
                if (unit->status_sets.current[1] & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_BLOOD_SUCK))
                    continue;
                break;
            }
        }
        distance = battle_ai_calculate_distance_between_units(ai->acting_unit, unit);
        if (distance < nearest_distance) {
            nearest_distance = distance;
            nearest_id = i;
        }
    }
    return nearest_id;
}
