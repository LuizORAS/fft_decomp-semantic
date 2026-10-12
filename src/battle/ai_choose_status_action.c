#include "fft/battle_ai.h"

/*
 * Choose or resume an action imposed by an AI-control status.
 *
 * Chicken takes precedence over Blood Suck, Confusion and Berserk. Return
 * -1 to suspend, 1 when selection completes, or 0 if none of these applies.
 */
s32 ai_choose_status_action(void) {
    ai_data_t* ai = &g_ai_data_base;
    battle_stats_t* unit;
    battle_stats_t* berserk_unit;
    ai_ability_entry_t* entry;
    s32 i, target_or_count;
    ai_coords_t coords;
    u8 status_flags;

    if (g_ai_data_base.resume_pending) {
        if (g_ai_data_base.action_selection_phase == AI_STATUS_SELECTION_CHICKEN)
            goto chicken;
        goto movement;
    }
    unit = g_ai_data_base.acting_unit;
    g_ai_data_base.movement_scenario = 0;
    status_flags = unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CHICKEN)];
    if (status_flags & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CHICKEN)) {
    chicken:
        if (ai_score_reachable_tiles() == -1) {
            ai->action_selection_phase = AI_STATUS_SELECTION_CHICKEN;
            return -1;
        }
        ai_clear_words(ai->tile_target_distance, sizeof(ai->tile_target_distance));
        ai_select_destination(255, AI_DESTINATION_DISTANCE_FIRST);
        coords = ai->candidate_coords[0];
        ai->selected_action.act_before_move = 0;
        goto save_movement;
    }
    if (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_BLOOD_SUCK)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_BLOOD_SUCK)) {
        target_or_count = ai_find_nearest_target(AI_NEAREST_WITHOUT_BLOOD_SUCK);
        entry = &ai->hardcoded_status_abilities[0];
        ai->hardcoded_status_abilities[0].packed_id |= g_ai_data_base.acting_unit_id << AI_ABILITY_ENTRY_UNIT_SHIFT;
    load_targeted_ability:
        ai_load_ability_entry(entry);
        {
            /* Direct member indexing changes this reverse clear. Keep the
             * target's base cursor and derive the field offset from its member. */
            u32 cursor;
            i = BATTLE_UNIT_SLOT_COUNT - 1;
            cursor = (u32)ai + i;
            do {
                *(u8*)(cursor + ((u32)&ai->targetability.live.unit_targetable - (u32)ai)) = 0;
                i--;
                cursor--;
            } while (i >= 0);
        }
        ai->targetability.live.unit_targetable[target_or_count] = 1;
        if (ai_choose_random_action())
            return 1;
    copy_target:
        ai_transfer_unit_coordinates(target_or_count, &ai->considered_unit_coords);
    movement:
        if (ai_fill_target_distance_grid(&ai->considered_unit_coords) == -1) {
            ai->action_selection_phase = AI_STATUS_SELECTION_TARGET_DISTANCE;
            return -1;
        }
        ai_select_destination(0x7fffffff, AI_DESTINATION_DISTANCE_FIRST);
        coords = ai->candidate_coords[ai->movement_scenario];
        ai->selected_action.act_before_move = 0;
    save_movement:
        /* Movement-only selection leaves the other action fields unchanged. */
        ai->selected_action.command.skillset_id = 0;
        ai->selected_action.coords = coords;
        goto success;
    }
    if (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CONFUSION)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CONFUSION)) {
        if (ai_select_active_units(AI_UNIT_FILTER_ANY, 255)) {
            for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
                ai->targetability.live.unit_targetable[i] = 0;
                if (!ai_check_target_type(i))
                    ai->targetability.live.unit_targetable[i] = ai->targetability.live.unit_active[i];
            }
            /* The aligned skillset/usage 16-bit value preserves target LHU/SH;
             * byte-only usage accesses change the instructions. */
            target_or_count = 0;
            for (i = 0; i < AI_ABILITY_LIST_COUNT; i++) {
                entry = &ai->ability_lists[ai->acting_unit_compact_index][i];
                if (entry->skillset == SKILLSET_ID_NONE)
                    break;
                if (entry->usage.policy.usable)
                    target_or_count++;
            }
            if (target_or_count) {
                do {
                    i = rand() % target_or_count;
                    {
                        s32 n;
                        for (n = 0; n < AI_ABILITY_LIST_COUNT; n++) {
                            entry = &ai->ability_lists[ai->acting_unit_compact_index][n];
                            if (entry->usage.policy.usable) {
                                if (i == 0)
                                    break;
                                i--;
                            }
                        }
                    }
                    ai_load_ability_entry(entry);
                    if (ai_choose_random_action())
                        return 1;
                    target_or_count--;
                    entry->usage.policy.usable = 0;
                } while (target_or_count);
            }
        }
        target_or_count = ai_find_nearest_target(AI_NEAREST_UNIT);
        goto copy_target;
    }
    if (status_flags & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_BERSERK)) {
        target_or_count = ai_find_nearest_target(AI_NEAREST_ENEMY);
        berserk_unit = g_ai_data_base.acting_unit;
        if (berserk_unit->primary_skillset < SKILLSET_ID_MONSTER_FIRST
            && !(berserk_unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_FROG)]
                & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FROG))) {
            entry = &ai->ability_lists[g_ai_data_base.acting_unit_compact_index][0];
        } else {
            entry = &ai->hardcoded_status_abilities[1];
            ai->hardcoded_status_abilities[1].packed_id |= ai->acting_unit_id << AI_ABILITY_ENTRY_UNIT_SHIFT;
        }
        goto load_targeted_ability;
    }
    return 0;
success:
    return 1;
}
