#include "fft/battle_ai.h"
#include "fft/battle_unit.h"

/*
 * Select the nearest unit admitted by the requested target filter.
 *
 * Manhattan-distance ties keep the first slot. If none qualify, return the
 * acting unit's ID. Mode 5 admits crystals before ordinary targetability
 * checks, while mode 4 shares mode 0's Blood Suck exclusion.
 */
s32 ai_find_nearest_target(ai_nearest_target_mode_e mode) {
    ai_data_t* ai = &g_ai_data_base;
    s32 nearest_distance = 0x7fffffff;
    s32 nearest_id = g_ai_data_base.acting_unit_id;
    s32 i;
    battle_stats_t* unit;
    ai_unit_decision_t* decision;
    s32 distance;

    for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
        unit = &g_battle_unit_stats[i];
        decision = &ai->unit_decisions[i];
        /* Crystals always qualify as healer-mode destinations. */
        if (mode != AI_NEAREST_HEALER_OR_CRYSTAL
            || !(unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_CRYSTAL)]
                & STATUS_BYTE_MASK(STATUS_ID_CRYSTAL))) {
            if (ai_is_unit_untargetable(i))
                continue;
            if (unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_PETRIFY)] & STATUS_BYTE_MASK(STATUS_ID_PETRIFY))
                continue;
            if (mode != AI_NEAREST_ELIGIBLE_ENEMY
                && (unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_DEAD)] & STATUS_BYTE_MASK(STATUS_ID_DEAD)))
                continue;
            switch (mode) {
            case AI_NEAREST_HEALER_OR_CRYSTAL:
                if (decision->enemy_flag || !(decision->targeting_flags_1 & AI_TARGETING_FLAG_1_CAN_HEAL_ALLIES))
                    continue;
                break;
            case AI_NEAREST_IMMINENT_ENEMY:
                if (!ai->targetability.live.unit_targetable[i])
                    continue;
                if (ai_should_exclude_from_imminent_enemies(i))
                    continue;
                if (ai_calculate_distance_between_units(ai->acting_unit, unit) < 2
                    && ai_calculate_height_difference_between_units(&ai->acting_unit_coords, unit) < -2)
                    continue;
                break;
            case AI_NEAREST_ENEMY:
                if (!decision->enemy_flag)
                    continue;
                break;
            case AI_NEAREST_ELIGIBLE_ENEMY:
                if (!decision->enemy_flag || (decision->targeting_flags_2 & AI_TARGET_DEAD_WITHOUT_RERAISE))
                    continue;
                if (!ai->targetability.live.unit_targetable[i] || !(decision->flags & AI_DECISION_CONSIDERED))
                    continue;
            case AI_NEAREST_WITHOUT_BLOOD_SUCK:
                if (unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_BLOOD_SUCK)]
                    & STATUS_BYTE_MASK(STATUS_ID_BLOOD_SUCK))
                    continue;
                break;
            }
        }
        distance = ai_calculate_distance_between_units(ai->acting_unit, unit);
        if (distance < nearest_distance) {
            nearest_distance = distance;
            nearest_id = i;
        }
    }
    return nearest_id;
}
