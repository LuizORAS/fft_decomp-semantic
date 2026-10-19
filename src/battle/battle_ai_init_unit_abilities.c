#include "fft/battle_ai.h"
#include "fft/battle_unit.h"
#include "fft/memory.h"

/* Rebuild a present unit's AI ability list (battle_ai_build_unit_ability_list) and the figures read from
 * it: its lowest and highest MP cost and the share, in quarters, of its abilities that cost MP, are
 * stopped by Silence or can be evaded (Monster Skill entries count only in the total). Each entry's search
 * mode is reset: an exhaustive origin scan for a linear or similar ability and for an evadable default
 * ability reaching less than 3 tiles, consumed origins for a default ability flagged CANNOT_HIT_CASTER or
 * CAN_TARGET_ENEMIES and for any ability with no area. The unit's five ENTD AI bytes and AI target tile
 * become its decision record; with the special-behaviour flag, targeting flag CONSERVE_CT makes it
 * retreat, and a focus unit found on the field makes it attack that unit (another team) or protect it
 * (its team), as Auto-Battle modes. The turn states, status changes and a Morbol transformation call it
 * again. */
void battle_ai_init_unit_abilities(s32 unit_id) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    battle_stats_t* unit = &g_battle_unit_stats[unit_id];
    battle_ai_unit_decision_t* decision;
    battle_ai_ability_entry_t* entry;
    s32 ability_count;
    s32 mp_count;
    s32 silence_count;
    s32 evade_count;
    s32 i;

    if (unit->entd_slot == BATTLE_ENTD_SLOT_NONE || ai->unit_compact_indices[unit_id] >= BATTLE_AI_COMPACT_UNIT_COUNT) {
        return;
    }
    battle_ai_build_unit_ability_list(unit_id);
    ability_count = 0;
    mp_count = 0;
    silence_count = 0;
    evade_count = 0;
    decision = &ai->unit_decisions[unit_id];
    decision->highest_mp_cost = 0;
    decision->lowest_mp_cost = 0xff;
    for (i = 0; i < BATTLE_AI_ABILITY_LIST_LENGTH; i++) {
        entry = &ai->ability_lists[ai->unit_compact_indices[unit_id]][i];
        if (entry->skillset == SKILLSET_ID_NONE) {
            break;
        }
        battle_ai_load_ability_entry(entry);
        ability_count++;
        /* The target reads and updates skillset/usage as one 16-bit value. */
        if (!entry->usage.policy.requires_monster_skill) {
            if (ai->considered_ability.ai_flags.word & BATTLE_AI_ABILITY_AFFECTED_BY_SILENCE) {
                silence_count++;
            }
            if (ai->considered_ability.ai_flags.word & BATTLE_AI_ABILITY_EVADEABLE) {
                evade_count++;
            }
            if (ai->considered_ability.mp_cost != 0) {
                mp_count++;
                if (decision->highest_mp_cost < ai->considered_ability.mp_cost) {
                    decision->highest_mp_cost = ai->considered_ability.mp_cost;
                }
                if (ai->considered_ability.mp_cost < decision->lowest_mp_cost) {
                    decision->lowest_mp_cost = ai->considered_ability.mp_cost;
                }
            }
        }
        entry->usage.flags &= BATTLE_AI_ABILITY_ENTRY_INIT_PRESERVE_MASK;
        if (ai->considered_ability.ai_flags.word
            & (BATTLE_AI_ABILITY_DIRECT_TRAJECTORY | BATTLE_AI_ABILITY_ARC_TRAJECTORY)) {
            continue;
        }
        if (ai->considered_ability.ai_flags.fields.flags_3 & BATTLE_AI_ABILITY_FLAG_3_EXHAUSTIVE_ORIGIN_MASK) {
            entry->usage.policy.exhaustive_origin_scan = 1;
        } else {
            if (ai->considered_ability.ability_id < ABILITY_ID_ITEM_FIRST) {
                if (ai->considered_ability.parameters.fields.range + ai->considered_ability.parameters.fields.aoe < 3
                    && (g_main_ability_range_data[ai->considered_ability.ability_id].flags_4
                        & ABILITY_SECONDARY_FLAG_4_EVADEABLE)) {
                    entry->usage.policy.exhaustive_origin_scan = 1;
                    continue;
                }
                if (g_main_ability_range_data[ai->considered_ability.ability_id].flags_2
                    & (ABILITY_SECONDARY_FLAG_2_CANNOT_HIT_CASTER | ABILITY_SECONDARY_FLAG_2_CAN_TARGET_ENEMIES)) {
                    entry->usage.policy.force_consume_target_tiles = 1;
                    continue;
                }
            }
            if (ai->considered_ability.parameters.fields.aoe == 0) {
                entry->usage.policy.force_consume_target_tiles = 1;
            }
        }
    }
    decision->silence_mod = battle_ai_calculate_ratio_times_4(ability_count, silence_count);
    decision->mp_ability_mod = battle_ai_calculate_ratio_times_4(ability_count, mp_count);
    decision->evade_mod = battle_ai_calculate_ratio_times_4(ability_count, evade_count);
    battle_mem_copy_bytes(&decision->flags, &unit->ai_flags, 5);
    decision->target.fields.x = unit->ai_target_x;
    decision->target.fields.y = unit->ai_target_y;
    decision->target.fields.elevation = unit->ai_flags >> 7;
    decision->target.fields.zero = 0;
    if (!(decision->flags & BATTLE_AI_DECISION_SPECIAL_BEHAVIOR)) {
        return;
    }
    if (decision->targeting_flags_2 & BATTLE_AI_TARGET_RUN_LIKE_A_RABBIT) {
        ai->unit_autobattle_modes[unit_id] = BATTLE_AUTO_BATTLE_RUN_LIKE_A_RABBIT;
        return;
    }
    if (!(decision->flags & BATTLE_AI_DECISION_FOCUS_TARGET)) {
        return;
    }
    for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
        if (g_battle_unit_stats[i].entd_slot == BATTLE_ENTD_SLOT_NONE
            || g_battle_unit_stats[i].unit_id != decision->main_target_id) {
            continue;
        }
        decision->main_target_id = i;
        if (!(decision->flags & BATTLE_AI_DECISION_AGGRESSIVE)) {
            return;
        }
        unit->auto_battle_target = i;
        if ((g_battle_unit_stats[i].initial_team_flags & BATTLE_TEAM_MASK)
            != (unit->initial_team_flags & BATTLE_TEAM_MASK)) {
            ai->unit_autobattle_modes[unit_id] = BATTLE_AUTO_BATTLE_FIGHT_FOR_LIFE;
        } else {
            ai->unit_autobattle_modes[unit_id] = BATTLE_AUTO_BATTLE_PROTECT_ALLIES;
        }
        return;
    }
    decision->flags &= ~BATTLE_AI_DECISION_SPECIAL_BEHAVIOR;
}
