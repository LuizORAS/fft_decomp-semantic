#include "fft/battle_ai.h"
#include "fft/battle_unit.h"

/* Mark each unit's usable abilities and derive the acting unit's AI hints.
 *
 * For every AI-considered unit, abilities blocked by Don't Act, Silence,
 * Innocent, MP, item stock or level are left unmarked; usable ones get entry
 * bit 0x8000 and feed the enemy-team counters and the per-unit helpful ratio.
 * The acting unit's list then drops buff-only abilities the enemies cannot
 * threaten, aggregates cancellable statuses, range limits and the random-use
 * threshold. action_taken clears the usable bits again.
 *
 * threshold holds first the status-check CT and later the enemy-count cutoff;
 * the single variable reproduces the target's register allocation. */
void ai_set_ability_considerations(s32 action_taken) {
    ai_data_t* ai;
    ai_considered_ability_t* ability;
    s32 delay;
    s32 enemy_harmful;
    s32 enemy_magical;
    s32 enemy_physical;
    s32 enemy_reflectable;
    s32 enemy_earth;
    s32 unit_harmful;
    s32 unit_helpful;
    s32 lowest_ratio;
    s32 lowest_unit;
    s32 unit_id;
    s32 i;
    ai_unit_decision_t* decision;
    battle_stats_t* unit;
    ai_ability_entry_t* entry;
    s32 ability_id;
    u8 menu;
    s32 best_level;
    s32 throw_id;
    s32 type_index;
    s32 item_id;
    s32 item_type;
    s32 lacking;
    s32 level;
    s32 total;
    s32 ratio;
    s32 threshold;
    s32 usable;
    s32 random_use;
    s32 range;

    enemy_harmful = 0;
    enemy_magical = 0;
    enemy_physical = 0;
    enemy_reflectable = 0;
    enemy_earth = 0;
    lowest_ratio = 0xff;
    lowest_unit = 0xff;
    ai = &g_ai_data_base;
    ability = &g_ai_data_base.considered_ability;
    unit_id = 0;
    do {
        if (ai_check_target_type(unit_id) == 0) {
            decision = &ai->unit_decisions[unit_id];
            unit = &g_battle_unit_stats[unit_id];
            unit_harmful = 0;
            unit_helpful = 0;
            if (unit_id == g_ai_data_base.acting_unit_id) {
                delay = 0;
            } else {
                delay = ai_calculate_clockticks_until_unit_acts(unit);
            }
            decision->targeting_flags_1
                &= ~(AI_TARGETING_FLAG_1_HAS_RANGED_ABILITY | AI_TARGETING_FLAG_1_CAN_HEAL_ALLIES);
            for (i = 0; i < AI_ABILITY_LIST_COUNT; i++) {
                entry = &ai->ability_lists[ai->unit_compact_indices[unit_id]][i];
                entry->usage.policy.usable = 0;
                if (entry->skillset == SKILLSET_ID_NONE) {
                    break;
                }
                ai_load_ability_entry(entry);
                ability_id = ability->ability_id;
                threshold = delay + ability->ct;
                if (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_FROG)]
                    & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FROG)) {
                    if (ability_id != ABILITY_ID_BLACK_MAGIC_FROG && ability_id != ABILITY_ID_FROG_ATTACK) {
                        continue;
                    }
                } else if (ability_id == ABILITY_ID_FROG_ATTACK) {
                    continue;
                }
                if (ai_is_status_active_through_delay(delay, unit, BATTLE_STATUS_ID_DONT_ACT)) {
                    continue;
                }
                if ((ability->ai_flags.halfwords.target_effect_flags & AI_ABILITY_AFFECTED_BY_SILENCE)
                    && ai_is_status_active_through_delay(threshold, unit, BATTLE_STATUS_ID_SILENCE)) {
                    continue;
                }
                if ((ability->ai_flags.halfwords.target_effect_flags & AI_ABILITY_AFFECTED_BY_FAITH)
                    && ai_is_status_active_through_delay(threshold, unit, BATTLE_STATUS_ID_INNOCENT)) {
                    continue;
                }
                if (ability->mp_cost > unit->mp) {
                    continue;
                }
                menu = g_main_action_menu_types_by_skillset[entry->skillset];
                if (menu == ACTION_MENU_TYPE_ITEM_INVENTORY || menu == ACTION_MENU_TYPE_KATANA_INVENTORY
                    || ability_id == ABILITY_ID_THROW_SHURIKEN || ability_id == ABILITY_ID_THROW_BALL) {
                    if (!(unit->initial_team_flags & BATTLE_TEAM_MASK)) {
                        if (g_main_item_quantities[ability->parameters.bytes.item_id] == 0) {
                            continue;
                        }
                    } else if (g_main_item_primary_data[ability->parameters.bytes.item_id].required_level
                        > unit->level) {
                        continue;
                    }
                } else if (menu == ACTION_MENU_TYPE_WEAPON_INVENTORY) {
                    if (unit_id != g_ai_data_base.acting_unit_id) {
                        continue;
                    }
                    best_level = -1;
                    for (throw_id = ABILITY_ID_THROW_KNIFE, type_index = 1; throw_id < ABILITY_ID_THROW_BALL;
                        throw_id++, type_index++) {
                        if (ai_load_known_ability_flag(
                                unit_id, ability->skillset, throw_id - (ABILITY_ID_THROW_KNIFE - 2))) {
                            item_type = g_main_throw_ability_item_types[type_index];
                            for (item_id = 0; item_id < ITEM_ID_COUNT; item_id++) {
                                if (g_main_item_primary_data[item_id].type == item_type) {
                                    level = g_main_item_primary_data[item_id].required_level;
                                    if (level >= best_level) {
                                        if (!(unit->initial_team_flags & BATTLE_TEAM_MASK)) {
                                            lacking = g_main_item_quantities[item_id] < 5;
                                        } else {
                                            lacking = unit->level < level;
                                        }
                                        if (!lacking) {
                                            best_level = level;
                                            ai->throw_ability_id = throw_id;
                                            ai->throw_weapon_id = item_id;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if (best_level == -1) {
                        continue;
                    }
                }
                entry->usage.policy.usable = 1;
                if (!entry->usage.policy.requires_monster_skill) {
                    if (decision->enemy_flag) {
                        if (ability->ai_flags.halfwords.target_effect_flags & AI_ABILITY_TARGET_ENEMIES) {
                            enemy_harmful++;
                            if (ability->ai_flags.halfwords.trajectory_usage_flags
                                & (AI_ABILITY_MAGICAL_ATTACK >> 16)) {
                                enemy_magical++;
                            }
                            if (ability->ai_flags.halfwords.trajectory_usage_flags
                                & (AI_ABILITY_PHYSICAL_ATTACK >> 16)) {
                                enemy_physical++;
                            }
                            if (ability->ai_flags.halfwords.target_effect_flags & AI_ABILITY_REFLECTABLE) {
                                enemy_reflectable++;
                            }
                            if (ability->element & BATTLE_ELEMENT_EARTH) {
                                enemy_earth++;
                            }
                        }
                    }
                    if ((ability->ai_flags.halfwords.target_effect_flags
                            & (AI_ABILITY_AFFECTS_HP | AI_ABILITY_TARGET_ALLIES))
                        == (AI_ABILITY_AFFECTS_HP | AI_ABILITY_TARGET_ALLIES)) {
                        decision->targeting_flags_1 |= AI_TARGETING_FLAG_1_CAN_HEAL_ALLIES;
                    }
                    if (ability->ai_flags.halfwords.trajectory_usage_flags
                        & ((AI_ABILITY_DIRECT_TRAJECTORY | AI_ABILITY_ARC_TRAJECTORY) >> 16)) {
                        decision->targeting_flags_1 |= AI_TARGETING_FLAG_1_HAS_RANGED_ABILITY;
                    }
                    if (ability->ai_flags.halfwords.target_effect_flags & AI_ABILITY_TARGET_ENEMIES) {
                        unit_harmful++;
                    }
                    if (ability->ai_flags.halfwords.target_effect_flags & AI_ABILITY_TARGET_ALLIES) {
                        unit_helpful++;
                    }
                }
            }
            if (decision->enemy_flag == 0) {
                total = unit_helpful + unit_harmful;
                if (total == 0) {
                    decision->targeting_flags_1 &= ~AI_TARGETING_FLAG_1_MOSTLY_SUPPORT;
                } else {
                    ratio = unit_helpful * AI_ABILITY_SHARE_SCALE / total;
                    if (ratio >= AI_SUPPORT_SHARE_THRESHOLD) {
                        decision->targeting_flags_1 |= AI_TARGETING_FLAG_1_MOSTLY_SUPPORT;
                    } else {
                        decision->targeting_flags_1 &= ~AI_TARGETING_FLAG_1_MOSTLY_SUPPORT;
                    }
                    if (ratio < lowest_ratio) {
                        lowest_unit = unit_id;
                        lowest_ratio = ratio;
                    }
                }
            }
        }
        unit_id++;
    } while (unit_id < BATTLE_UNIT_SLOT_COUNT);

    usable = 0;
    if (lowest_unit != 0xff) {
        ai->unit_decisions[lowest_unit].targeting_flags_1 &= ~AI_TARGETING_FLAG_1_MOSTLY_SUPPORT;
    }
    random_use = 0;
    decision = g_ai_data_base.acting_unit_decision;
    ai->has_mp_recovery_ability = 0;
    ai->targetability.live.target_setting_flags &= ~AI_TARGET_SETTING_HAS_HOLY_WATER;
    decision->ally_ability_min_reach = 0xff;
    decision->enemy_ability_min_reach = 0xff;
    threshold = enemy_harmful / 8;
    for (i = 0; i < BATTLE_STATUS_BYTE_COUNT; i++) {
        ai->status_to_cancel[i] = 0;
    }
    for (i = 0; i < AI_ABILITY_LIST_COUNT; i++) {
        entry = &ai->ability_lists[g_ai_data_base.acting_unit_compact_index][i];
        if (entry->skillset == SKILLSET_ID_NONE) {
            break;
        }
        ai_load_ability_entry(entry);
        if (entry->usage.policy.usable) {
            if (threshold >= enemy_magical) {
                ai_disable_ability_if_only_inflicts_status(entry, BATTLE_STATUS_ID_SHELL);
            }
            if (threshold >= enemy_physical) {
                ai_disable_ability_if_only_inflicts_status(entry, BATTLE_STATUS_ID_PROTECT);
            }
            if (threshold >= enemy_reflectable) {
                ai_disable_ability_if_only_inflicts_status(entry, BATTLE_STATUS_ID_REFLECT);
            }
            if (enemy_earth <= 0) {
                ai_disable_ability_if_only_inflicts_status(entry, BATTLE_STATUS_ID_FLOAT);
            }
            if (entry->usage.policy.usable) {
                usable++;
                if (ability->ai_flags.halfwords.target_effect_flags & AI_ABILITY_RANDOM_USE) {
                    random_use++;
                }
                if (ability->ability_id == ABILITY_ID_ITEM_HOLY_WATER) {
                    ai->targetability.live.target_setting_flags |= AI_TARGET_SETTING_HAS_HOLY_WATER;
                }
                if (ability->ai_flags.halfwords.target_effect_flags & AI_ABILITY_CANCEL_STATUS) {
                    for (throw_id = 0; throw_id < BATTLE_STATUS_BYTE_COUNT; throw_id++) {
                        ai->status_to_cancel[throw_id] |= ability->parameters.bytes.status_infliction[throw_id];
                    }
                }
                if ((ability->ai_flags.halfwords.target_effect_flags
                        & (AI_ABILITY_AFFECTS_MP | AI_ABILITY_TARGET_ENEMIES))
                    == AI_ABILITY_AFFECTS_MP) {
                    ai->has_mp_recovery_ability = 1;
                }
                range = ability->parameters.bytes.range + ability->parameters.bytes.aoe;
                if ((ability->ai_flags.halfwords.target_effect_flags & AI_ABILITY_TARGET_ENEMIES)
                    && range < decision->enemy_ability_min_reach) {
                    decision->enemy_ability_min_reach = range;
                }
                if (ability->ai_flags.halfwords.target_effect_flags & AI_ABILITY_TARGET_ALLIES) {
                    if (g_main_action_menu_types_by_skillset[ability->skillset] == ACTION_MENU_TYPE_ITEM_INVENTORY) {
                        range = (range + 1) / 2;
                    }
                    if (range < decision->ally_ability_min_reach) {
                        decision->ally_ability_min_reach = range;
                    }
                }
                if (entry->usage.policy.force_consume_evaluated_origins) {
                    entry->usage.policy.consume_evaluated_origins = 1;
                } else if (!entry->usage.policy.exhaustive_origin_scan) {
                    if (ability->ct >= g_ai_data_base.acting_unit_remaining_clockticks
                        || ai_classify_ability_effect(g_ai_data_base.acting_unit_id, 1) == AI_ABILITY_EFFECT_NONE) {
                        entry->usage.policy.consume_evaluated_origins = 1;
                    } else {
                        entry->usage.policy.consume_evaluated_origins = 0;
                    }
                }
                if (action_taken != 0) {
                    entry->usage.policy.usable = 0;
                }
            }
        }
    }
    if (decision->enemy_ability_min_reach != 0xff) {
        decision->enemy_ability_min_reach += g_ai_data_base.acting_unit_move;
    }
    if (decision->ally_ability_min_reach == 0xff) {
        decision->ally_ability_min_reach = 0;
    }
    decision->ally_ability_min_reach += g_ai_data_base.acting_unit_move;
    if (usable != 0) {
        ai->random_use_threshold = random_use * AI_ABILITY_SHARE_SCALE / usable;
    }
}
