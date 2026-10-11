#include "fft/battle.h"

/*
 * Build the AI's attack-origin and movable tile masks for a linear ability.
 */
void battle_ai_evaluate_linear_ability_behaviors(void) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    s32 i;
    s32 j;
    s32 x;
    s32 unit_id;
    s32 depth;
    battle_stats_t* unit;
    battle_stats_t* pos;
    s32 jumping;
    u8 status;

    for (i = 0; i < BATTLE_AI_GRID_ROW_COUNT; i++) {
        ai->attack_origin_tiles[0][i] = 0;
        ai->attack_origin_tiles[1][i] = 0;
    }
    if (ai->considered_ability.ai_flags.word & BATTLE_AI_ABILITY_ARC_TRAJECTORY) {
        battle_ai_transfer_halfword_values(
            ai->movable_tiles, ai->reachable_tiles[ai->movement_scenario][0], sizeof(ai->movable_tiles));
    } else {
        for (unit_id = 0; unit_id < BATTLE_UNIT_SLOT_COUNT; unit_id++) {
            if (ai->targetability.live.unit_targetable[unit_id]) {
                battle_ai_transfer_unit_coordinates(unit_id, &ai->considered_unit_coords);
                if (ai->current_action.command.targeting_type == BATTLE_TARGETING_TYPE_UNIT)
                    battle_ai_build_targetable_tile_mask(BATTLE_AI_TILE_MASK_RANGE, 0);
                else
                    battle_ai_build_targetable_tile_mask(BATTLE_AI_TILE_MASK_RANGE_PLUS_AOE, 0);
                for (i = 0; i < ai->map_max_y; i++) {
                    ai->attack_origin_tiles[0][i] |= ai->targetable_tiles[0][i];
                    ai->attack_origin_tiles[1][i] |= ai->targetable_tiles[1][i];
                }
            }
        }
        for (i = 0; i < ai->map_max_y; i++) {
            ai->movable_tiles[0][i] = ai->reachable_tiles[ai->movement_scenario][0][i] & ai->attack_origin_tiles[0][i];
            ai->movable_tiles[1][i] = ai->reachable_tiles[ai->movement_scenario][1][i] & ai->attack_origin_tiles[1][i];
            ai->attack_origin_tiles[0][i] = 0;
            ai->attack_origin_tiles[1][i] = 0;
        }
        for (i = 0; i < MAP_TERRAIN_LAYER_COUNT; i++) {
            for (j = 0; j < ai->map_max_y; j++) {
                unit_id = ai->movable_tiles[i][j];
                if (unit_id == 0)
                    continue;
                for (x = 0; x < ai->map_max_x; x++) {
                    if ((unit_id << x) & 0x8000) {
                        if (ai->water_penalty
                            && (depth = g_battle_map_tile_data[j * ai->map_max_x + x + (i * MAP_TILES_PER_LAYER)]
                                            .depth_half_height.value
                                       >> MAP_TILE_DEPTH_SHIFT)
                                >= 2) {
                            ai->movable_tiles[i][j] ^= 0x8000 >> x;
                        } else if (g_main_action_menu_types_by_skillset[ai->considered_ability.skillset]
                            == ACTION_MENU_TYPE_ELEMENTS) {
                            ai->considered_unit_coords.bytes.x = x;
                            ai->considered_unit_coords.bytes.y = j;
                            ai->considered_unit_coords.bytes.elevation = i;
                            if (!battle_ai_can_elemental_ability_be_used(&ai->considered_unit_coords))
                                ai->movable_tiles[i][j] ^= 0x8000 >> x;
                        }
                    }
                }
            }
        }
        if (ai->considered_ability.ai_flags.word & BATTLE_AI_ABILITY_REQUIRES_MONSTER_SKILL) {
            battle_ai_build_monster_skill_tile_mask();
            for (i = 0; i < ai->map_max_y; i++) {
                ai->movable_tiles[0][i] &= ai->targetable_tiles[0][i];
                ai->movable_tiles[1][i] &= ai->targetable_tiles[1][i];
            }
        }
    }
    for (unit_id = 0; unit_id < BATTLE_UNIT_SLOT_COUNT; unit_id++) {
        if (ai->targetability.live.unit_targetable[unit_id]) {
            battle_ai_transfer_unit_coordinates(unit_id, &ai->considered_unit_coords);
            if (ai->current_action.command.targeting_type == BATTLE_TARGETING_TYPE_UNIT)
                battle_ai_build_targetable_tile_mask(BATTLE_AI_TILE_MASK_EXPLICIT, 0);
            else
                battle_ai_build_targetable_tile_mask(BATTLE_AI_TILE_MASK_AOE, 0);
            for (i = 0; i < ai->map_max_y; i++) {
                ai->attack_origin_tiles[0][i] |= ai->targetable_tiles[0][i];
                ai->attack_origin_tiles[1][i] |= ai->targetable_tiles[1][i];
            }
        }
    }
    if (ai->considered_ability.ai_flags.word & BATTLE_AI_ABILITY_FOLLOW_TARGET) {
        for (unit_id = 0; unit_id < BATTLE_UNIT_SLOT_COUNT; unit_id++) {
            if (ai->targetability.live.unit_targetable[unit_id]) {
                unit = &g_battle_unit_stats[unit_id];
                status = unit->status_sets.current[0];
                /* Keeps the position reads on a copy of the loop pointer (`move a1,a2`) that cse2 would fold. */
                pos = unit;
                __asm__("" : "=r"(pos) : "0"(pos));
                jumping = status & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_JUMP);
                if (ai->current_action.command.targeting_type == BATTLE_TARGETING_TYPE_TILE || jumping) {
                    ai->attack_origin_tiles[pos->position.higher_elevation][pos->position.y] ^= 0x8000 >> pos->x;
                }
            }
        }
    }
    for (i = 0; i < MAP_TERRAIN_LAYER_COUNT; i++) {
        for (j = 0; j < ai->map_max_y; j++) {
            ai->attack_origin_tiles[i][j] &= ai->walkable_tiles[i][j];
        }
    }
    ai->attack_origin_tile_count = battle_ai_count_flagged_tiles_within_map_bounds(ai->attack_origin_tiles);
    ai->movable_tile_count = battle_ai_count_flagged_tiles_within_map_bounds(ai->movable_tiles);
}
