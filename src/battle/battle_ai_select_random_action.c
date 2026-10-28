#include "fft/battle_ai.h"
#include "fft/memory.h"

/* Make a random valid use of the considered ability the selected action, for the statuses that take
 * control. Math Skill first draws a learned, AI-usable, Calculator-eligible spell from one of the four
 * source skillsets, then a learned type (CT, Level, Exp, Height) and multiplier (Prime Number, 5, 4,
 * 3). A map-tile ability takes a random reachable tile that battle_ai_can_use_ability_at_coords
 * accepts. Any other takes a random origin around the targetable units
 * (battle_ai_build_origin_and_target_tiles) and a random tile in range, facing it, aimed at a unit
 * or at the tile (an ability that can follow its target picks one of the two at random). Returns 1
 * after copying the action, 0 when no origin and target work. */
s32 battle_ai_select_random_action(void) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    s32 value, skillset;
    u16* abilities;
    s32 index_or_count;

    g_battle_ai_data_base.current_action.act_before_move = 0;
    g_battle_ai_data_base.current_action.command.skillset_id = ai->considered_ability.skillset;
    g_battle_ai_data_base.current_action.command.ability_id = ai->considered_ability.ability_id;
    /* Item selection updates only the low byte of the 16-bit command field. */
    *(u8*)&g_battle_ai_data_base.current_action.command.item_or_equipment
        = ai->considered_ability.parameters.fields.item_id;
    if (g_main_action_menu_types_by_skillset[ai->considered_ability.skillset] == ACTION_MENU_TYPE_ARITHMETICKS) {
        do {
            skillset = g_battle_ai_math_source_skillsets[rand() % BATTLE_AI_MATH_SOURCE_SKILLSET_COUNT];
            abilities = main_ability_store_skillset_abilities(skillset, SKILLSET_ABILITY_FILTER_ACTION);
            index_or_count = rand() % RSM_SKILLSET_SLOT_FIRST;
            value = abilities[index_or_count];
        } while (value == 0 || !(g_main_ability_data[value].ai_flags.flags_4 & BATTLE_AI_ABILITY_FLAG_4_USABLE_BY_AI)
            || !(g_main_ability_range_data[value].flags_3 & ABILITY_SECONDARY_FLAG_3_CALCULATOR_ELIGIBLE)
            || !battle_ai_is_ability_known(ai->acting_unit_id, skillset, index_or_count));
        ai->current_action.command.ability_id = value;
        do {
            value = rand() % CALCULATOR_ATTRIBUTE_ABILITY_COUNT;
        } while (!battle_ai_is_ability_known(ai->acting_unit_id, ai->considered_ability.skillset, value));
        ai->current_action.command.calculator_type = value + ABILITY_ID_MATH_FIRST;
        do {
            value = rand() % CALCULATOR_MULTIPLE_ABILITY_COUNT + CALCULATOR_ATTRIBUTE_ABILITY_COUNT;
        } while (!battle_ai_is_ability_known(ai->acting_unit_id, ai->considered_ability.skillset, value));
        ai->current_action.command.calculator_multiplier = value + ABILITY_ID_MATH_FIRST;
    }
    if (ai->considered_ability.ai_flags.word & BATTLE_AI_ABILITY_TARGET_MAP_TILES) {
        battle_mem_copy_u16_values(ai->targetable_tiles, ai->reachable_tiles[0], sizeof(ai->targetable_tiles));
        ai->origin_tile_count = battle_ai_count_flagged_tiles_within_map_bounds(ai->targetable_tiles);
        if (!ai->origin_tile_count)
            return 0;
        do {
            if (!battle_ai_take_random_tile(ai->targetable_tiles, &ai->current_action.coords, ai->origin_tile_count))
                return 0;
            ai->origin_tile_count--;
            if (battle_ai_can_use_ability_at_coords(&ai->current_action.coords)) {
                ai->current_action.wait_facing_hint = BATTLE_AI_DIRECTION_OVERLAP;
                goto selected;
            }
        } while (ai->origin_tile_count);
    } else {
        /* Removing this single-attempt loop changes register allocation. */
        do {
            if (ai->considered_ability.ai_flags.word & BATTLE_AI_ABILITY_FOLLOW_TARGET)
                ai->current_action.command.targeting_type = rand() % 2 + BATTLE_TARGETING_TYPE_TILE;
            else
                ai->current_action.command.targeting_type = BATTLE_TARGETING_TYPE_TILE;
            battle_ai_build_origin_and_target_tiles();
            if (!ai->origin_tile_count) {
                if (ai->current_action.command.targeting_type == BATTLE_TARGETING_TYPE_UNIT) {
                    ai->current_action.command.targeting_type = BATTLE_TARGETING_TYPE_TILE;
                    battle_ai_build_origin_and_target_tiles();
                }
                if (!ai->origin_tile_count)
                    break;
            }
            do {
                if (!battle_ai_take_random_tile(ai->origin_tiles, &ai->current_action.coords, ai->origin_tile_count))
                    return 0;
                ai->origin_tile_count--;
                /* Keep the original count even when a rejected target bit is removed. */
                index_or_count = battle_ai_count_target_tiles_in_range();
                if (index_or_count) {
                    while (
                        battle_ai_take_random_tile(ai->targetable_tiles, &ai->considered_unit_coords, index_or_count)) {
                        ai->current_action.command.target_position.vx = ai->considered_unit_coords.fields.x;
                        ai->current_action.command.target_position.vz = ai->considered_unit_coords.fields.y;
                        ai->current_action.command.target_position.vy = ai->considered_unit_coords.fields.elevation;
                        ai->current_action.wait_facing_hint = battle_ai_find_direction_of_target(
                            &ai->considered_unit_coords, &ai->current_action.coords);
                        if (ai->current_action.command.targeting_type == BATTLE_TARGETING_TYPE_UNIT
                            && (ai->current_action.command.target_id
                                   = battle_ai_find_unit_at_coordinates(&ai->considered_unit_coords))
                                == BATTLE_UNIT_ID_NONE)
                            continue;
                        goto selected;
                    }
                }
            } while (ai->origin_tile_count);
        } while (0);
    }
    return 0;
selected: /* shared success tail */
    battle_mem_copy_u16_values(&ai->selected_action, &ai->current_action, sizeof(battle_ai_action_data_t));
    return 1;
}
