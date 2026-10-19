#include "fft/battle_ai.h"
#include "fft/battle_unit.h"
#include "fft/memory.h"

/* Look for an ability that removes status_id (Dead, Petrify or Blood Suck) from unit_id, for Protect
 * allies. Dead and Petrify are tried only when the decision record marks the unit dead without Reraise
 * or petrified, and Blood Suck only when it marks the unit jumping (QUIRKS.md). The search ranks the
 * acting unit's abilities that cancel the status, leaving out Faith ones on an innately Innocent unit,
 * reflectable ones on an innately Reflecting unit and Silence-bound ones while the actor is Silenced.
 * Returns 2 with the best ranked action selected; 0 when no ability can remove it (none, or all too
 * costly with no way to restore MP); 1 to go on to the next status; -1 to suspend. */
s32 battle_ai_evaluate_status_cancellation(s32 unit_id, s32 status_id) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    battle_stats_t* unit = &g_battle_unit_stats[unit_id];
    battle_ai_unit_decision_t* decision;
    battle_ai_ability_entry_t* entry;
    s32 flags;
    s32 status_byte;

    if (g_battle_ai_data_base.resume_pending != 0) {
        /* Resume: the target jumps into the middle of the ability loop. */
        goto evaluate_ability;
    }

    decision = &ai->unit_decisions[unit_id];
    g_battle_ai_status_candidate_count = 0;
    g_battle_ai_status_insufficient_mp = 0;
    g_battle_ai_status_sufficient_mp = 0;

    switch (status_id) {
    case BATTLE_STATUS_ID_DEAD:
        if (!(decision->flags & BATTLE_AI_DECISION_DEAD_WITHOUT_RERAISE))
            return 1;
        break;
    case BATTLE_STATUS_ID_PETRIFY:
        if (!(decision->flags & BATTLE_AI_DECISION_PETRIFIED))
            return 1;
        break;
    case BATTLE_STATUS_ID_BLOOD_SUCK:
        if (!(decision->flags & BATTLE_AI_DECISION_JUMPING))
            return 1;
        break;
    }

    if (battle_ai_select_active_units(BATTLE_AI_UNIT_FILTER_SPECIFIC, unit_id) != 0) {
        for (ai->ability_counter = 0; ai->ability_counter < BATTLE_AI_ABILITY_LIST_LENGTH; ai->ability_counter++) {
            entry = &ai->ability_lists[ai->acting_unit_compact_index][ai->ability_counter];
            if (entry->skillset == SKILLSET_ID_NONE)
                break;
            battle_ai_load_ability_entry(entry);
            flags = ai->considered_ability.ai_flags.word;
            if (!(flags & BATTLE_AI_ABILITY_CANCEL_STATUS))
                continue;

            status_byte = status_id / 8;
            if (!(ai->considered_ability.parameters.fields.status_infliction[status_byte] & (0x80 >> (status_id % 8))))
                continue;
            if ((flags & BATTLE_AI_ABILITY_AFFECTED_BY_FAITH)
                && (unit->status_sets.innate[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_INNOCENT)]
                    & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_INNOCENT)))
                continue;
            if ((flags & BATTLE_AI_ABILITY_REFLECTABLE)
                && (unit->status_sets.innate[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_REFLECT)]
                    & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_REFLECT)))
                continue;
            if ((flags & BATTLE_AI_ABILITY_AFFECTED_BY_SILENCE)
                && (ai->acting_unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_SILENCE)]
                    & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_SILENCE)))
                continue;

            if (ai->acting_unit->mp < ai->considered_ability.mp_cost)
                g_battle_ai_status_insufficient_mp = 1;
            else
                g_battle_ai_status_sufficient_mp = 1;
            g_battle_ai_status_candidate_count++;

        evaluate_ability:
            if (battle_ai_evaluate_ability_outcome() == -1)
                return -1;
        }
    }

    if (ai->ranked_actions[0].rank.base_hit_percent != 0) {
        battle_mem_copy_u16_values(&ai->selected_action, &ai->ranked_actions[0], sizeof(battle_ai_action_data_t));
        return 2;
    }
    if (g_battle_ai_status_candidate_count == 0)
        return 0;
    if (g_battle_ai_status_insufficient_mp != 0 && ai->has_mp_recovery_ability == 0
        && g_battle_ai_status_sufficient_mp == 0)
        return 0;
    return 1;
}
