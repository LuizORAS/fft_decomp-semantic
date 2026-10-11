#include "fft/battle.h"

/*
 * Score target tiles from the current simulated action origin.
 *
 * Intersect range with remaining target candidates, apply direct/arc path checks,
 * and resume suspended priority evaluation at its saved tile. A -1 result
 * suspends this pass; 0 ends it. Coordinate restoration belongs to callers.
 */
s32 battle_ai_evaluate_target_tiles(void) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    s32 x, y, level;
    u16 row;
    battle_ai_coords_t* origin;
    s32 i;
    map_tile_t* tile;
    u16* target_rows;
    u16* origin_rows;
    s32 tile_mask;
    u32 level_address;
    u8 target_id;

    if (g_battle_ai_data_base.resume_pending && g_battle_ai_data_base.search_resume_flag) {
        x = (u16)g_battle_ai_data_base.current_action.command.target_position.vx;
        level = (u16)g_battle_ai_data_base.current_action.command.target_position.vy;
        y = (u16)g_battle_ai_data_base.current_action.command.target_position.vz;
        row = ai->targetable_tiles[level][y];
        goto evaluate_priority;
    }
    if (VSync(1) >= BATTLE_AI_DECISION_SCANLINE_LIMIT) {
        ai->search_resume_flag = 0;
        return -1;
    }
    ai->resume_pending = 0;
    if (ai->current_action.act_before_move)
        origin = &ai->acting_unit_coords;
    else
        origin = &ai->current_action.coords;
    battle_ai_move_temp_unit_to_coords(origin);
    if (ai->considered_ability.parameters.bytes.range) {
        if (battle_target_set_panels_for_action(&ai->current_action.command) == BATTLE_TARGET_PANELS_NONE)
            return 0;
    } else {
        tile = g_battle_map_tile_data;
        for (i = 0; i < MAP_TILE_COUNT; i++, tile++)
            tile->ceiling_depth_and_marks.value &= ~MAP_TILE_FLAG_ABILITY_RANGE;
        g_battle_map_tile_data[origin->bytes.y * ai->map_max_x + origin->bytes.x + (origin->bytes.elevation << 8)]
            .ceiling_depth_and_marks.value |= MAP_TILE_FLAG_ABILITY_RANGE;
    }

    /* Field-derived level bases keep both row pointers in the target order;
     * typed level indexing adds instructions to this intersection loop. */
    for (level = 0, level_address = (u32)ai; level < MAP_TERRAIN_LAYER_COUNT;
        level++, level_address += BATTLE_AI_GRID_ROW_COUNT * sizeof(u16)) {
        for (y = 0; y < ai->map_max_y; y++) {
            target_rows = (u16*)(level_address + ((u32)&ai->targetable_tiles - (u32)ai));
            origin_rows = (u16*)(level_address + ((u32)&ai->attack_origin_tiles - (u32)ai));
            row = origin_rows[y];
            target_rows[y] = 0;
            if (row != 0) {
                for (x = 0; x < ai->map_max_x; x++) {
                    /* Keep the membership test and update in one working mask. */
                    tile_mask = row << x;
                    if (tile_mask & 0x8000) {
                        if ((g_battle_map_tile_data[y * ai->map_max_x + x + (level << 8)].ceiling_depth_and_marks.value
                                >> MAP_TILE_FLAG_ABILITY_RANGE_SHIFT)
                            & 1) {
                            tile_mask = 0x8000;
                            target_rows[y] |= tile_mask >> x;
                        }
                    }
                }
            }
        }
    }
    for (level = 0; level < MAP_TERRAIN_LAYER_COUNT; level++) {
        for (y = 0; y < ai->map_max_y; y++) {
            row = ai->targetable_tiles[level][y];
            if (row != 0) {
                for (x = 0; x < ai->map_max_x; x++) {
                    if ((row << x) & 0x8000) {
                        ai->current_action.command.target_position.vx = x;
                        ai->current_action.command.target_position.vz = y;
                        ai->current_action.command.target_position.vy = level;
                        if (ai->current_action.command.targeting_type == BATTLE_TARGETING_TYPE_UNIT
                            || (ai->considered_ability.ai_flags.word
                                & (BATTLE_AI_ABILITY_DIRECT_TRAJECTORY | BATTLE_AI_ABILITY_ARC_TRAJECTORY))) {
                            target_id = battle_ai_has_any_unit_on_target_panel();
                            ai->current_action.command.target_id = target_id;
                            if (target_id == BATTLE_UNIT_ID_NONE)
                                goto remove_useful_origin;
                        }
                        if (ai->considered_ability.ai_flags.word & BATTLE_AI_ABILITY_ARC_TRAJECTORY) {
                            if (battle_effect_check_arc_trajectory_between_units(
                                    ai->acting_unit_id, ai->current_action.command.target_id)
                                == 0)
                                continue;
                        } else if (ai->considered_ability.ai_flags.word & BATTLE_AI_ABILITY_DIRECT_TRAJECTORY) {
                            if ((g_main_action_menu_types_by_skillset[ai->considered_ability.skillset]
                                        != ACTION_MENU_TYPE_ITEM_INVENTORY
                                    || battle_ai_calculate_distance_between_units(
                                           ai->acting_unit, &g_battle_unit_stats[ai->current_action.command.target_id])
                                        >= 2)
                                && battle_effect_check_direct_trajectory_between_units(
                                       ai->acting_unit_id, ai->current_action.command.target_id)
                                    == 0)
                                continue;
                        }
                        if (ai->targetability.live.target_setting_flags
                            & BATTLE_AI_TARGET_SETTING_CONSUME_EVALUATED_ORIGINS)
                            ai->useful_on_caster_flag = 1;
                        else
                            ai->useful_on_caster_flag = 0;
                    evaluate_priority:
                        if (battle_ai_check_set_highest_unit_priority() == -1) {
                            ai->search_resume_flag = 1;
                            return -1;
                        }
                        if (ai->ability_processing_done == 0)
                            return 0;
                    remove_useful_origin:
                        if (ai->useful_on_caster_flag) {
                            ai->attack_origin_tiles[level][y] ^= 0x8000 >> x;
                            ai->attack_origin_tile_count--;
                            if (ai->attack_origin_tile_count == 0)
                                break;
                        }
                    }
                }
            }
        }
    }
    return 0;
}
