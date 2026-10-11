#include "fft/battle.h"

/* The target reads the AI flag word as two halfwords (lhu +0x0c/+0x0e). */
#define AI_FLAGS_LO(a) (((u16*)&a->ai_flags)[0])
#define AI_FLAGS_HI(a) (((u16*)&a->ai_flags)[1])

enum {
    /* Helpful abilities out of 128 that make a non-enemy a supporter. */
    BATTLE_AI_SUPPORTER_RATIO_MIN = 77,
};

/* Mark the usable entries of every unit's AI ability list and derive the acting unit's hints
 * (battle_ai_build_command). For each unit still in play, an entry is usable unless the unit's Frog
 * state does not match it (a Frog keeps only Frog and Frog Attack), Don't Act lasts until its turn,
 * Silence or Innocent lasts until the ability would resolve for an ability they stop, its MP is short,
 * or an item, katana or thrown item is missing (the player's team checks the party's stock, others the
 * item's required level); only the acting unit's generic Throw entry is usable, with the best throwable
 * it has (stock of 5 or more for the player). Usable abilities set the unit's HEALER and RANGED_ABILITY targeting
 * flags, make it a SUPPORTER when at least 77 of 128 of them help allies (never the least helpful
 * non-enemy), and count the acting unit's enemies' harmful, magical, physical, reflectable and earth
 * abilities. The acting unit then drops its abilities that only add Shell, Protect or Reflect when at
 * most 1/8 of the enemies' harmful abilities are magical, physical or reflectable, and Float when none
 * is earth; it gathers the statuses it can cancel, its random-use share (of 128), whether it can
 * restore MP or use Holy Water, and its reach toward an enemy or ally target (Move plus its shortest
 * ability of each kind, items halved). An entry's origins are consumed when its CT reaches the acting
 * unit's remaining clock ticks or it has no effect from the unit's side. Once the unit has acted, none of
 * its entries is usable.
 *
 * threshold holds first the status-check CT and later the enemy-count cutoff; the single variable
 * reproduces the target's register allocation. */
void battle_ai_set_ability_considerations(s32 action_taken) {
    battle_ai_data_t* ai;
    battle_ai_considered_ability_t* ability;
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
    battle_ai_unit_decision_t* decision;
    battle_stats_t* unit;
    battle_ai_ability_entry_t* entry;
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
    u16 flags;

    enemy_harmful = 0;
    enemy_magical = 0;
    enemy_physical = 0;
    enemy_reflectable = 0;
    enemy_earth = 0;
    lowest_ratio = 0xff;
    lowest_unit = 0xff;
    ai = &g_battle_ai_data_base;
    ability = &g_battle_ai_data_base.considered_ability;
    unit_id = 0;
    do {
        if (battle_ai_is_unit_out_of_play(unit_id) == 0) {
            decision = &ai->unit_decisions[unit_id];
            unit = &g_battle_unit_stats[unit_id];
            unit_harmful = 0;
            unit_helpful = 0;
            if (unit_id == g_battle_ai_data_base.acting_unit_id) {
                delay = 0;
            } else {
                delay = battle_ai_calculate_clockticks_until_unit_acts(unit);
            }
            decision->targeting_flags_1
                &= ~(BATTLE_AI_TARGETING_FLAG_1_HAS_RANGED_ABILITY | BATTLE_AI_TARGETING_FLAG_1_CAN_HEAL_ALLIES);
            for (i = 0; i < 0x22; i++) {
                entry = &ai->ability_lists[ai->unit_compact_indices[unit_id]][i];
                entry->skillset_flags.packed &= ~(BATTLE_AI_ABILITY_ENTRY_USABLE << 8);
                if (entry->skillset_flags.bytes.skillset == SKILLSET_ID_NONE) {
                    break;
                }
                battle_ai_load_ability_entry(entry);
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
                if (battle_ai_is_status_active_through_delay(delay, unit, BATTLE_STATUS_ID_DONT_ACT)) {
                    continue;
                }
                if ((AI_FLAGS_LO(ability) & BATTLE_AI_ABILITY_AFFECTED_BY_SILENCE)
                    && battle_ai_is_status_active_through_delay(threshold, unit, BATTLE_STATUS_ID_SILENCE)) {
                    continue;
                }
                if ((AI_FLAGS_LO(ability) & BATTLE_AI_ABILITY_AFFECTED_BY_FAITH)
                    && battle_ai_is_status_active_through_delay(threshold, unit, BATTLE_STATUS_ID_INNOCENT)) {
                    continue;
                }
                if (ability->mp_cost > unit->mp) {
                    continue;
                }
                menu = g_main_action_menu_types_by_skillset[entry->skillset_flags.bytes.skillset];
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
                    if (unit_id != g_battle_ai_data_base.acting_unit_id) {
                        continue;
                    }
                    best_level = -1;
                    for (throw_id = ABILITY_ID_THROW_KNIFE, type_index = 1; throw_id < ABILITY_ID_THROW_BALL;
                        throw_id++, type_index++) {
                        if (battle_ai_load_known_ability_flag(
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
                entry->skillset_flags.packed |= (BATTLE_AI_ABILITY_ENTRY_USABLE << 8);
                if (!(entry->skillset_flags.packed & (BATTLE_AI_ABILITY_ENTRY_REQUIRES_MONSTER_SKILL << 8))) {
                    if (decision->enemy_flag) {
                        if (AI_FLAGS_LO(ability) & BATTLE_AI_ABILITY_TARGET_ENEMIES) {
                            enemy_harmful++;
                            if (AI_FLAGS_HI(ability) & (BATTLE_AI_ABILITY_MAGICAL_ATTACK >> 16)) {
                                enemy_magical++;
                            }
                            if (AI_FLAGS_HI(ability) & (BATTLE_AI_ABILITY_PHYSICAL_ATTACK >> 16)) {
                                enemy_physical++;
                            }
                            if (AI_FLAGS_LO(ability) & BATTLE_AI_ABILITY_REFLECTABLE) {
                                enemy_reflectable++;
                            }
                            if (ability->element & BATTLE_ELEMENT_EARTH) {
                                enemy_earth++;
                            }
                        }
                    }
                    if ((AI_FLAGS_LO(ability) & (BATTLE_AI_ABILITY_AFFECTS_HP | BATTLE_AI_ABILITY_TARGET_ALLIES))
                        == (BATTLE_AI_ABILITY_AFFECTS_HP | BATTLE_AI_ABILITY_TARGET_ALLIES)) {
                        decision->targeting_flags_1 |= BATTLE_AI_TARGETING_FLAG_1_CAN_HEAL_ALLIES;
                    }
                    if (AI_FLAGS_HI(ability)
                        & ((BATTLE_AI_ABILITY_DIRECT_TRAJECTORY | BATTLE_AI_ABILITY_ARC_TRAJECTORY) >> 16)) {
                        decision->targeting_flags_1 |= BATTLE_AI_TARGETING_FLAG_1_HAS_RANGED_ABILITY;
                    }
                    if (AI_FLAGS_LO(ability) & BATTLE_AI_ABILITY_TARGET_ENEMIES) {
                        unit_harmful++;
                    }
                    if (AI_FLAGS_LO(ability) & BATTLE_AI_ABILITY_TARGET_ALLIES) {
                        unit_helpful++;
                    }
                }
            }
            if (decision->enemy_flag == 0) {
                total = unit_helpful + unit_harmful;
                if (total == 0) {
                    decision->targeting_flags_1 &= ~BATTLE_AI_TARGETING_FLAG_1_MOSTLY_SUPPORT;
                } else {
                    ratio = (unit_helpful << 7) / total;
                    if (ratio >= 0x4d) {
                        decision->targeting_flags_1 |= BATTLE_AI_TARGETING_FLAG_1_MOSTLY_SUPPORT;
                    } else {
                        decision->targeting_flags_1 &= ~BATTLE_AI_TARGETING_FLAG_1_MOSTLY_SUPPORT;
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
        ai->unit_decisions[lowest_unit].targeting_flags_1 &= ~BATTLE_AI_TARGETING_FLAG_1_MOSTLY_SUPPORT;
    }
    random_use = 0;
    decision = g_battle_ai_data_base.acting_unit_decision;
    ai->has_mp_recovery_ability = 0;
    ai->targetability.live.target_setting_flags &= ~BATTLE_AI_TARGET_SETTING_HAS_HOLY_WATER;
    decision->ally_reach = 0xff;
    decision->enemy_reach = 0xff;
    threshold = enemy_harmful / 8;
    for (i = 0; i < 5; i++) {
        ai->status_to_cancel[i] = 0;
    }
    for (i = 0; i < BATTLE_AI_ABILITY_LIST_LENGTH; i++) {
        entry = &ai->ability_lists[g_battle_ai_data_base.acting_unit_compact_index][i];
        if (entry->skillset_flags.bytes.skillset == SKILLSET_ID_NONE) {
            break;
        }
        battle_ai_load_ability_entry(entry);
        if (entry->skillset_flags.packed & (BATTLE_AI_ABILITY_ENTRY_USABLE << 8)) {
            if (threshold >= enemy_magical) {
                battle_ai_disable_ability_if_only_inflicts_status(entry, BATTLE_STATUS_ID_SHELL);
            }
            if (threshold >= enemy_physical) {
                battle_ai_disable_ability_if_only_inflicts_status(entry, BATTLE_STATUS_ID_PROTECT);
            }
            if (threshold >= enemy_reflectable) {
                battle_ai_disable_ability_if_only_inflicts_status(entry, BATTLE_STATUS_ID_REFLECT);
            }
            if (enemy_earth <= 0) {
                battle_ai_disable_ability_if_only_inflicts_status(entry, BATTLE_STATUS_ID_FLOAT);
            }
            if (entry->skillset_flags.packed & (BATTLE_AI_ABILITY_ENTRY_USABLE << 8)) {
                usable++;
                if (AI_FLAGS_LO(ability) & BATTLE_AI_ABILITY_RANDOM_USE) {
                    random_use++;
                }
                if (ability->ability_id == ABILITY_ID_ITEM_HOLY_WATER) {
                    ai->targetability.live.target_setting_flags |= BATTLE_AI_TARGET_SETTING_HAS_HOLY_WATER;
                }
                if (AI_FLAGS_LO(ability) & BATTLE_AI_ABILITY_CANCEL_STATUS) {
                    for (throw_id = 0; throw_id < 5; throw_id++) {
                        ai->status_to_cancel[throw_id] |= ability->parameters.bytes.status_infliction[throw_id];
                    }
                }
                if ((AI_FLAGS_LO(ability) & (BATTLE_AI_ABILITY_AFFECTS_MP | BATTLE_AI_ABILITY_TARGET_ENEMIES))
                    == BATTLE_AI_ABILITY_AFFECTS_MP) {
                    ai->has_mp_recovery_ability = 1;
                }
                range = ability->parameters.bytes.range + ability->parameters.bytes.aoe;
                if ((AI_FLAGS_LO(ability) & BATTLE_AI_ABILITY_TARGET_ENEMIES) && range < decision->enemy_reach) {
                    decision->enemy_reach = range;
                }
                if (AI_FLAGS_LO(ability) & BATTLE_AI_ABILITY_TARGET_ALLIES) {
                    if (g_main_action_menu_types_by_skillset[ability->skillset] == ACTION_MENU_TYPE_ITEM_INVENTORY) {
                        range = (range + 1) / 2;
                    }
                    if (range < decision->ally_reach) {
                        decision->ally_reach = range;
                    }
                }
                flags = entry->skillset_flags.packed;
                if (flags & (BATTLE_AI_ABILITY_ENTRY_FORCE_CONSUME_TARGET_TILES << 8)) {
                    entry->skillset_flags.packed = flags | (BATTLE_AI_ABILITY_ENTRY_CONSUME_TARGET_TILES << 8);
                } else if (!(flags & (BATTLE_AI_ABILITY_ENTRY_EXHAUSTIVE_ORIGIN_SCAN << 8))) {
                    if (ability->ct >= g_battle_ai_data_base.acting_unit_remaining_clockticks
                        || battle_ai_classify_ability_effect(g_battle_ai_data_base.acting_unit_id, 1)
                            == BATTLE_AI_ABILITY_EFFECT_NONE) {
                        entry->skillset_flags.packed |= (BATTLE_AI_ABILITY_ENTRY_CONSUME_TARGET_TILES << 8);
                    } else {
                        entry->skillset_flags.packed &= ~(BATTLE_AI_ABILITY_ENTRY_CONSUME_TARGET_TILES << 8);
                    }
                }
                if (action_taken != 0) {
                    entry->skillset_flags.packed &= ~(BATTLE_AI_ABILITY_ENTRY_USABLE << 8);
                }
            }
        }
    }
    if (decision->enemy_reach != 0xff) {
        decision->enemy_reach += g_battle_ai_data_base.acting_unit_move;
    }
    if (decision->ally_reach == 0xff) {
        decision->ally_reach = 0;
    }
    decision->ally_reach += g_battle_ai_data_base.acting_unit_move;
    if (usable != 0) {
        ai->random_use_threshold = (random_use << 7) / usable;
    }
}
