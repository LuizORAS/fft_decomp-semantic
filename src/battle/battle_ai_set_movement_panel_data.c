#include "fft/battle.h"
#include "psx/libetc.h"

/* Build the AI walkable and reachable tile rows for the acting unit.
 *
 * A fresh decision (resume_pending 0) sets the water penalty, rebuilds the
 * walkable rows, clears the scenario rows and spreads movement unless the
 * unit has moved or cannot move. The reachable rows then drop occupied tiles
 * and, unless the unit is disabled, the four listed coordinates. Returns -1
 * when the frame budget is spent. i and j are reused as the spread unit id
 * and the teleport-range distances, as the target's register assignment
 * shows. */
s32 battle_ai_set_movement_panel_data(s32 movement_taken) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    battle_stats_t* unit;
    map_tile_t* tile;
    s32 level;
    s32 y;
    s32 x;
    s32 i;
    u8 mount_info;
    s32 j;
    battle_ai_coords_t* coords;

    if (g_battle_ai_data_base.resume_pending == 0) {
        if ((g_battle_ai_data_base.acting_unit->movement_abilities[2]
                & (BATTLE_MOVEMENT_SET_3_MOVE_IN_WATER | BATTLE_MOVEMENT_SET_3_WALK_ON_WATER
                    | BATTLE_MOVEMENT_SET_3_FLOAT))
            || (g_battle_ai_data_base.acting_unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_FLOAT)]
                & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FLOAT))) {
            g_battle_ai_data_base.water_penalty = 0;
        } else {
            g_battle_ai_data_base.water_penalty = 1;
        }
        battle_ai_clear_words(ai->walkable_tiles, sizeof(ai->walkable_tiles));
        for (level = 0; level < MAP_TERRAIN_LAYER_COUNT; level++) {
            for (y = 0; y < ai->map_max_y; y++) {
                for (x = 0; x < ai->map_max_x; x++) {
                    tile = &g_battle_map_tile_data[level * MAP_TILES_PER_LAYER + y * ai->map_max_x + x];
                    if (!(tile->flags.value & MAP_TILE_FLAG_BLOCKED)
                        && (tile->surface.value & MAP_SURFACE_MASK) != MAP_SURFACE_CROSS_SECTION) {
                        ai->walkable_tiles[level][y] |= 0x8000 >> x;
                    }
                }
            }
        }
        ai->movement_scenario = 0;
        for (j = 0; j < BATTLE_AI_GRID_ROW_COUNT; j++) {
            ai->reachable_tiles[ai->movement_scenario][0][j] = 0;
            ai->reachable_tiles[ai->movement_scenario][1][j] = 0;
        }
        if (movement_taken != 0
            || (ai->acting_unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DONT_MOVE)]
                & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DONT_MOVE))) {
            ai->reachable_tiles[0][ai->acting_unit_coords.bytes.elevation][ai->acting_unit_coords.bytes.y]
                = 0x8000 >> ai->acting_unit_coords.bytes.x;
            return 0;
        }
        mount_info = ai->acting_unit->mount_info;
        if (mount_info & BATTLE_MOUNT_INFO_FLAG_RIDER) {
            i = mount_info & BATTLE_MOUNT_INFO_PARTNER_ID_MASK;
        } else {
            i = ai->acting_unit_id;
        }
        battle_move_set_reachable_tiles(
            i, ai->acting_unit_coords.bytes.x, ai->acting_unit_coords.bytes.y, ai->acting_unit_coords.bytes.elevation);
    }
    if (VSync(1) >= BATTLE_AI_DECISION_SCANLINE_LIMIT) {
        return -1;
    }
    ai->resume_pending = 0;
    for (level = 0; level < MAP_TERRAIN_LAYER_COUNT; level++) {
        for (y = 0; y < ai->map_max_y; y++) {
            for (x = 0; x < ai->map_max_x; x++) {
                tile = &g_battle_map_tile_data[level * MAP_TILES_PER_LAYER + y * ai->map_max_x + x];
                if (ai->acting_unit->movement_abilities[1] & BATTLE_MOVEMENT_SET_2_TELEPORT) {
                    if (!(ai->acting_unit->mount_info & BATTLE_MOUNT_INFO_FLAG_RIDER)) {
                        i = ai->acting_unit_coords.bytes.x - x;
                        if (i < 0) {
                            i = -i;
                        }
                        j = ai->acting_unit_coords.bytes.y - y;
                        if (j < 0) {
                            j = -j;
                        }
                        if (ai->acting_unit_move < i + j) {
                            continue;
                        }
                    }
                }
                if (tile->ceiling_depth_and_marks.value & MAP_TILE_FLAG_MOVE_DESTINATION) {
                    ai->reachable_tiles[0][level][y] |= 0x8000 >> x;
                }
            }
        }
    }
    for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
        unit = &g_battle_unit_stats[i];
        if (unit->entd_slot != BATTLE_ENTD_SLOT_NONE && i != ai->acting_unit_id
            && !(unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CRYSTAL)]
                & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CRYSTAL))
            && !(unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_TREASURE)]
                & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_TREASURE))) {
            ai->reachable_tiles[0][unit->position.higher_elevation][unit->position.y] &= ~(0x8000 >> unit->x);
        }
    }
    ai->reachable_tiles[0][ai->acting_unit_coords.bytes.elevation][ai->acting_unit_coords.bytes.y]
        |= 0x8000 >> ai->acting_unit_coords.bytes.x;
    if (*(u32*)&ai->acting_unit->status_sets.current[0]
        & (BATTLE_STATUS_PACKED_MASK(BATTLE_STATUS_ID_CONFUSION)
            | BATTLE_STATUS_PACKED_MASK(BATTLE_STATUS_ID_BERSERK))) {
        return 0;
    }
    for (i = 0; i < BATTLE_AI_KNOWN_TRAP_COUNT; i++) {
        coords = &ai->known_trap_coords[i];
        if (coords->bytes.x == 0xff) {
            break;
        }
        ai->reachable_tiles[0][coords->bytes.elevation][coords->bytes.y] &= ~(0x8000 >> coords->bytes.x);
    }
    ai->reachable_tiles[0][ai->acting_unit_coords.bytes.elevation][ai->acting_unit_coords.bytes.y]
        |= 0x8000 >> ai->acting_unit_coords.bytes.x;
    return 0;
}
