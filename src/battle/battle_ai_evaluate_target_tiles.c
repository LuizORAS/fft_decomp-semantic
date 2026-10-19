#include "fft/battle_ai.h"
#include "fft/battle_effect.h"
#include "fft/battle_map.h"
#include "fft/battle_target.h"
#include "fft/battle_unit.h"

/* Evaluate the considered ability from the current origin (the unit's own tile when it acts before
 * moving) against each target tile in range. The range comes from battle_target_set_panels_for_action
 * (none in range ends the pass), or is the origin tile alone for an ability without range. A
 * unit-aimed or trajectory ability needs a unit on the tile (battle_ai_find_unit_on_target_tile), and
 * an arc or direct one a clear path (an item for an adjacent unit skips that check); then the action
 * is simulated and scored (battle_ai_score_current_action). A settled tile
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
    if (ai->considered_ability.parameters.fields.range) {
        if (battle_target_set_panels_for_action(&ai->current_action.command) == BATTLE_TARGET_PANELS_NONE)
            return 0;
    } else {
        tile = g_battle_map_tile_data;
        for (i = 0; i < MAP_TILE_COUNT; i++, tile++)
            tile->ceiling_depth_and_marks.ability_range = 0;
        g_battle_map_tile_data[origin->fields.y * ai->map_max_x + origin->fields.x
            + (origin->fields.elevation * MAP_TILES_PER_LAYER)]
            .ceiling_depth_and_marks.ability_range = 1;
    }

    /* Field-derived level bases keep both row pointers in the target order;
     * typed level indexing adds instructions to this intersection loop. */
    for (level = 0, level_address = (u32)ai; level < MAP_TERRAIN_LAYER_COUNT;
        level++, level_address += BATTLE_AI_GRID_ROW_COUNT * sizeof(u16)) {
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
                        if (g_battle_map_tile_data[y * ai->map_max_x + x + (level * MAP_TILES_PER_LAYER)]
                                .ceiling_depth_and_marks.ability_range) {
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
                            target_id = battle_ai_find_unit_on_target_tile();
                            ai->current_action.command.target_id = target_id;
                            /* An empty tile is dropped by the flag the previous tile left. */
                            if (target_id == BATTLE_UNIT_ID_NONE)
                                goto drop_settled_target;
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
                        if (ai->targetability.live.target_setting_flags & BATTLE_AI_TARGET_SETTING_CONSUME_TARGET_TILES)
                            ai->target_tile_settled = 1;
                        else
                            ai->target_tile_settled = 0;
                    evaluate_priority:
                        if (battle_ai_score_current_action() == -1) {
                            ai->search_resume_flag = 1;
                            return -1;
                        }
                        if (ai->ability_processing_done == 0)
                            return 0;
                    drop_settled_target:
                        if (ai->target_tile_settled) {
                            ai->target_tiles[level][y] ^= 0x8000 >> x;
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
