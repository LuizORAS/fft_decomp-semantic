#include "fft/battle.h"

/* Evaluate the considered ability from the current origin (the unit's own tile when it acts before
 * moving) against each target tile in range. The range comes from battle_target_set_panels_for_action
 * (none in range ends the pass), or is the origin tile alone for an ability without range. A
 * unit-aimed or trajectory ability needs a unit on the tile (battle_ai_find_unit_on_target_tile), and
 * an arc or direct one a clear path (an item for an adjacent unit skips that check); then the action
 * is simulated and scored (battle_ai_check_set_highest_unit_priority). A settled tile
 * (target_tile_settled, always set with CONSUME_TARGET_TILES) leaves target_tiles. Returns -1 to
 * suspend (also past 441 horizontal blanks), else 0, and stops early when the scoring clears
 * ability_processing_done. */
s32 battle_ai_evaluate_target_tiles(void) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    s32 x, y, level;
    u16 row;
    battle_ai_coords_t* origin;
    s32 i;
    map_tile_t* tile;
    u16* target_rows;
    u16* origin_rows;
    s32 path_clear;
    s32 tile_mask;
    u32 level_address;
    u8 target_id;

    /* Field-derived byte stages preserve the original level/row address
     * order; flattened typed indexing changes the generated instructions. */
    if (g_battle_ai_data_base.decision_state && g_battle_ai_data_base.search_resume_flag) {
        x = g_battle_ai_data_base.current_action.target_x;
        level = g_battle_ai_data_base.current_action.target_elevation;
        y = g_battle_ai_data_base.current_action.target_y;
        {
            u32 scan_level_address = level * (18 * sizeof(u16)) + (u32)ai;
            u32 scan_row_address = y * sizeof(u16);
            row = *(u16*)(scan_level_address + scan_row_address + ((u32)&ai->targetable_tiles - (u32)ai));
        }
        goto evaluate_priority;
    }
    if (VSync(1) >= 441) {
        ai->search_resume_flag = 0;
        return -1;
    }
    ai->decision_state = 0;
    if (ai->current_action.act_before_move)
        origin = &ai->acting_unit_coords;
    else
        origin = &ai->current_action.coords;
    battle_ai_move_temp_unit_to_coords(origin);
    if (ai->considered_ability.parameters.bytes.range) {
        if (battle_target_set_panels_for_action(&ai->current_action.unit_id) == 3)
            return 0;
    } else {
        tile = g_battle_map_tile_data;
        for (i = 0; i < 512; i++, tile++)
            tile->ceiling_depth_and_marks &= ~MAP_TILE_FLAG_ABILITY_RANGE;
        g_battle_map_tile_data[origin->bytes.y * ai->map_max_x + origin->bytes.x + (origin->bytes.elevation << 8)]
            .ceiling_depth_and_marks |= MAP_TILE_FLAG_ABILITY_RANGE;
    }

    /* Field-derived level bases keep both row pointers in the target order;
     * typed level indexing adds instructions to this intersection loop. */
    for (level = 0, level_address = (u32)ai; level < 2; level++, level_address += 18 * sizeof(u16)) {
        for (y = 0; y < ai->map_max_y; y++) {
            target_rows = (u16*)(level_address + ((u32)&ai->targetable_tiles - (u32)ai));
            origin_rows = (u16*)(level_address + ((u32)&ai->target_tiles - (u32)ai));
            row = origin_rows[y];
            target_rows[y] = 0;
            if (row != 0) {
                for (x = 0; x < ai->map_max_x; x++) {
                    /* Keep the membership test and update in one working mask. */
                    tile_mask = row << x;
                    if (tile_mask & 0x8000) {
                        if ((g_battle_map_tile_data[y * ai->map_max_x + x + (level << 8)].ceiling_depth_and_marks
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
    for (level = 0; level < 2; level++) {
        for (y = 0; y < ai->map_max_y; y++) {
            /* Typed flattened indexing combines the level/row arithmetic. */
            {
                u32 scan_level_address = level * (18 * sizeof(u16)) + (u32)ai;
                u32 scan_row_address = y * sizeof(u16);
                row = *(u16*)(scan_level_address + scan_row_address + ((u32)&ai->targetable_tiles - (u32)ai));
            }
            if (row != 0) {
                for (x = 0; x < ai->map_max_x; x++) {
                    if ((row << x) & 0x8000) {
                        ai->current_action.target_x = x;
                        ai->current_action.target_y = y;
                        ai->current_action.target_elevation = level;
                        if (ai->current_action.targeting_type == BATTLE_ACTION_TARGET_UNIT
                            || (ai->considered_ability.ai_flags.word
                                & (BATTLE_AI_ABILITY_DIRECT_TRAJECTORY | BATTLE_AI_ABILITY_ARC_TRAJECTORY))) {
                            target_id = battle_ai_find_unit_on_target_tile();
                            ai->current_action.target_id = target_id;
                            /* An empty tile is dropped by the flag the previous tile left. */
                            if (target_id == BATTLE_UNIT_ID_NONE)
                                goto drop_settled_target;
                        }
                        if (ai->considered_ability.ai_flags.word & BATTLE_AI_ABILITY_ARC_TRAJECTORY) {
                            path_clear = battle_effect_check_arc_trajectory_between_units(
                                ai->acting_unit_id, ai->current_action.target_id);
                        } else if (ai->considered_ability.ai_flags.word & BATTLE_AI_ABILITY_DIRECT_TRAJECTORY) {
                            if (g_main_action_menu_types_by_skillset[ai->considered_ability.skillset]
                                    == ACTION_MENU_TYPE_ITEM_INVENTORY
                                && battle_ai_calculate_distance_between_units(
                                       ai->acting_unit, &g_battle_unit_stats[ai->current_action.target_id])
                                    < 2)
                                goto set_self_effect;
                            path_clear = battle_effect_check_direct_trajectory_between_units(
                                ai->acting_unit_id, ai->current_action.target_id);
                        } else {
                            goto set_self_effect;
                        }
                        if (path_clear == 0)
                            continue;
                    set_self_effect:
                        if (ai->targetability.live.target_setting_flags & BATTLE_AI_TARGET_SETTING_CONSUME_TARGET_TILES)
                            ai->target_tile_settled = 1;
                        else
                            ai->target_tile_settled = 0;
                    evaluate_priority:
                        if (battle_ai_check_set_highest_unit_priority() == -1) {
                            ai->search_resume_flag = 1;
                            return -1;
                        }
                        if (ai->ability_processing_done == 0)
                            return 0;
                    drop_settled_target:
                        if (ai->target_tile_settled) {
                            /* A typed level pointer moves the field offset before
                             * the AI base addition; retain the target's order. */
                            {
                                u32 origin_address = level * (18 * sizeof(u16)) + (u32)ai;
                                u16* origin_row = (u16*)(origin_address + ((u32)&ai->target_tiles - (u32)ai));
                                origin_row[y] ^= 0x8000 >> x;
                            }
                            ai->target_tile_count--;
                            if (ai->target_tile_count == 0)
                                break;
                        }
                    }
                }
            }
        }
    }
    return 0;
}
