#include "fft/battle_ai.h"

/*
 * Choose a random action from the candidate movement and target masks.
 *
 * Math Skill first chooses learned ability and parameter combinations. Return
 * 1 after copying the current action, including fields unchanged by this pass,
 * or 0 when selection fails.
 */
s32 ai_choose_random_action(void) {
    ai_data_t* ai = &g_ai_data_base;
    s32 value, skillset;
    u16* abilities;
    s32 index_or_count;

    g_ai_data_base.current_action.act_before_move = 0;
    g_ai_data_base.current_action.command.skillset_id = ai->considered_ability.skillset;
    g_ai_data_base.current_action.command.ability_id = ai->considered_ability.ability_id;
    /* Item selection updates only the low byte of the 16-bit command field. */
    *(u8*)&g_ai_data_base.current_action.command.item_or_equipment = ai->considered_ability.parameters.bytes.item_id;
    if (g_main_action_menu_types_by_skillset[ai->considered_ability.skillset] == ACTION_MENU_TYPE_ARITHMETICKS) {
        do {
            skillset = g_ai_math_source_skillsets[rand() % AI_MATH_SOURCE_SKILLSET_COUNT];
            abilities = main_ability_store_skillset_abilities(skillset, SKILLSET_ABILITY_FILTER_ACTION);
            index_or_count = rand() % HUMAN_SKILLSET_ACTION_SLOT_COUNT;
            value = abilities[index_or_count];
        } while (value == 0 || !(g_main_ability_data[value].ai_flags.flags_4 & AI_ABILITY_FLAG_4_USABLE_BY_AI)
            || !(g_main_ability_range_data[value].flags_3 & ABILITY_SECONDARY_FLAG_3_CALCULATOR_ELIGIBLE)
            || !ai_load_known_ability_flag(ai->acting_unit_id, skillset, index_or_count));
        ai->current_action.command.ability_id = value;
        do {
            value = rand() % CALCULATOR_ATTRIBUTE_ABILITY_COUNT;
        } while (!ai_load_known_ability_flag(ai->acting_unit_id, ai->considered_ability.skillset, value));
        ai->current_action.command.calculator_type = value + ABILITY_ID_MATH_FIRST;
        do {
            value = rand() % CALCULATOR_MULTIPLE_ABILITY_COUNT + CALCULATOR_ATTRIBUTE_ABILITY_COUNT;
        } while (!ai_load_known_ability_flag(ai->acting_unit_id, ai->considered_ability.skillset, value));
        ai->current_action.command.calculator_multiplier = value + ABILITY_ID_MATH_FIRST;
    }
    if (ai->considered_ability.ai_flags.word & AI_ABILITY_TARGET_MAP_TILES) {
        ai_transfer_halfword_values(ai->targetable_tiles, ai->reachable_tiles[0], sizeof(ai->targetable_tiles));
        ai->movable_tile_count = ai_count_flagged_tiles_within_map_bounds(ai->targetable_tiles);
        if (!ai->movable_tile_count)
            return 0;
        do {
            if (!ai_take_random_tile(ai->targetable_tiles, &ai->current_action.coords, ai->movable_tile_count))
                return 0;
            ai->movable_tile_count--;
            if (ai_check_ability_use_at_coords(&ai->current_action.coords)) {
                ai->current_action.wait_facing_hint = AI_DIRECTION_OVERLAP;
                goto selected;
            }
        } while (ai->movable_tile_count);
    } else {
        /* Removing this single-attempt loop changes register allocation. */
        do {
            if (ai->considered_ability.ai_flags.word & AI_ABILITY_FOLLOW_TARGET)
                ai->current_action.command.targeting_type = rand() % 2 + BATTLE_TARGETING_TYPE_TILE;
            else
                ai->current_action.command.targeting_type = BATTLE_TARGETING_TYPE_TILE;
            ai_evaluate_linear_ability_behaviors();
            if (!ai->movable_tile_count) {
                if (ai->current_action.command.targeting_type == BATTLE_TARGETING_TYPE_UNIT) {
                    ai->current_action.command.targeting_type = BATTLE_TARGETING_TYPE_TILE;
                    ai_evaluate_linear_ability_behaviors();
                }
                if (!ai->movable_tile_count)
                    break;
            }
            do {
                if (!ai_take_random_tile(ai->movable_tiles, &ai->current_action.coords, ai->movable_tile_count))
                    return 0;
                ai->movable_tile_count--;
                /* Keep the original count even when a rejected target bit is removed. */
                index_or_count = ai_calculate_ability_range();
                if (index_or_count) {
                    while (ai_take_random_tile(ai->targetable_tiles, &ai->considered_unit_coords, index_or_count)) {
                        ai->current_action.command.target_position.vx = ai->considered_unit_coords.bytes.x;
                        ai->current_action.command.target_position.vz = ai->considered_unit_coords.bytes.y;
                        ai->current_action.command.target_position.vy = ai->considered_unit_coords.bytes.elevation;
                        ai->current_action.wait_facing_hint
                            = ai_find_direction_of_target(&ai->considered_unit_coords, &ai->current_action.coords);
                        if (ai->current_action.command.targeting_type == BATTLE_TARGETING_TYPE_UNIT
                            && (ai->current_action.command.target_id
                                   = ai_find_unit_at_coordinates(&ai->considered_unit_coords))
                                == BATTLE_UNIT_ID_NONE)
                            continue;
                        goto selected;
                    }
                }
            } while (ai->movable_tile_count);
        } while (0);
    }
    return 0;
selected: /* shared success tail */
    ai_transfer_halfword_values(&ai->selected_action, &ai->current_action, sizeof(ai_action_data_t));
    return 1;
}
