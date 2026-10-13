#include "fft/battle_ai.h"
#include "fft/battle_unit.h"
#include "fft/memory.h"

/* Evaluate the actor's abilities for removing one status from a target.
 *
 * The return value distinguishes suspension (-1), no usable ability (0), a
 * retry-worthy result (1), and a selected action (2). The Blood Suck path
 * deliberately tests the transient Jump flag, preserving the shipped AI
 * quirk.
 */
s32 ai_evaluate_status_cancellation(s32 unit_id, s32 status_id) {
    ai_data_t* ai = &g_ai_data_base;
    unit_stats_t* unit = &g_battle_unit_stats[unit_id];
    ai_unit_decision_t* decision;
    ai_ability_entry_t* entry;
    s32 flags;
    s32 status_byte;

    if (g_ai_data_base.resume_pending != 0) {
        /* Resume: the target jumps into the middle of the ability loop. */
        goto evaluate_ability;
    }

    decision = &ai->unit_decisions[unit_id];
    g_ai_status_candidate_count = 0;
    g_ai_status_insufficient_mp = 0;
    g_ai_status_sufficient_mp = 0;

    switch (status_id) {
    case STATUS_ID_DEAD:
        if (!(decision->flags & AI_DECISION_TRANSIENT_DEAD))
            return 1;
        break;
    case STATUS_ID_PETRIFY:
        if (!(decision->flags & AI_DECISION_TRANSIENT_PETRIFIED))
            return 1;
        break;
    case STATUS_ID_BLOOD_SUCK:
        if (!(decision->flags & AI_DECISION_TRANSIENT_JUMP))
            return 1;
        break;
    }

    if (ai_select_active_units(AI_UNIT_FILTER_SPECIFIC, unit_id) != 0) {
        for (ai->ability_counter = 0; ai->ability_counter < AI_ABILITY_LIST_COUNT; ai->ability_counter++) {
            entry = &ai->ability_lists[ai->acting_unit_compact_index][ai->ability_counter];
            if (entry->skillset == SKILLSET_ID_NONE)
                break;
            ai_load_ability_entry(entry);
            flags = ai->considered_ability.ai_flags.word;
            if (!(flags & AI_ABILITY_CANCEL_STATUS))
                continue;

            status_byte = status_id / 8;
            if (!(ai->considered_ability.parameters.fields.status_infliction[status_byte] & (0x80 >> (status_id % 8))))
                continue;
            if ((flags & AI_ABILITY_AFFECTED_BY_FAITH)
                && (unit->status_sets.innate[STATUS_BYTE_INDEX(STATUS_ID_INNOCENT)]
                    & STATUS_BYTE_MASK(STATUS_ID_INNOCENT)))
                continue;
            if ((flags & AI_ABILITY_REFLECTABLE)
                && (unit->status_sets.innate[STATUS_BYTE_INDEX(STATUS_ID_REFLECT)]
                    & STATUS_BYTE_MASK(STATUS_ID_REFLECT)))
                continue;
            if ((flags & AI_ABILITY_AFFECTED_BY_SILENCE)
                && (ai->acting_unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_SILENCE)]
                    & STATUS_BYTE_MASK(STATUS_ID_SILENCE)))
                continue;

            if (ai->acting_unit->mp < ai->considered_ability.mp_cost)
                g_ai_status_insufficient_mp = 1;
            else
                g_ai_status_sufficient_mp = 1;
            g_ai_status_candidate_count++;

        evaluate_ability:
            if (ai_evaluate_ability_outcome() == -1)
                return -1;
        }
    }

    if (ai->ranked_actions[0].rank.base_hit_percent != 0) {
        ai_copy_u16_values(&ai->selected_action, &ai->ranked_actions[0], sizeof(ai_action_data_t));
        return 2;
    }
    if (g_ai_status_candidate_count == 0)
        return 0;
    if (g_ai_status_insufficient_mp != 0 && ai->has_mp_recovery_ability == 0 && g_ai_status_sufficient_mp == 0)
        return 0;
    return 1;
}
