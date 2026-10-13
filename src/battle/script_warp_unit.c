#include "fft/battle_script.h"
#include "fft/battle_unit.h"
#include "fft/script.h"
#include "psx/types.h"

/*
 * WarpUnit event instruction: move an ENTD unit to a new tile and facing.
 *
 * Parameters: 0x00 ENTD unit id (16-bit value), 0x02 x, 0x03 y, 0x04 higher
 * elevation, 0x05 facing.
 */
void script_warp_unit(const event_warp_unit_parameters_t* instruction) {
    /* Separate instruction and placement pointers change register allocation. */
    union {
        const event_warp_unit_parameters_t* instruction;
        const event_unit_placement_t* placement;
    } cursor;
    battle_stats_t* unit;
    s32 battle_id;
    s32 misc_id;
    s32 result;
    s16 unit_id;
    map_tile_coordinates_t tile;
    unit_animation_state_t* rotation_state;

    cursor.instruction = instruction;
    unit_id = script_read_s16(cursor.instruction->unit_id_le);
    cursor.placement = &cursor.instruction->placement;
    unit = battle_unit_find_stats_by_entd_unit_id(unit_id, &result);
    battle_id = 0;
    if (result != -2 && result < 0) {
        return;
    }
    for (; battle_id < EVENT_UNIT_SLOT_COUNT; battle_id++) {
        if (battle_unit_get_stats_from_battle_id(battle_id) == unit) {
            break;
        }
    }
    unit->x = cursor.placement->x;
    unit->position.y = cursor.placement->y;
    unit->position.higher_elevation = cursor.placement->elevation;
    unit->position.facing = cursor.placement->facing;
    misc_id = battle_unit_get_misc_id_by_battle_id(battle_id);
    if (misc_id == -1) {
        return;
    }
    tile.x = unit->x;
    tile.layer = unit->position.higher_elevation;
    tile.y = unit->position.y;
    rotation_state = &g_battle_unit_misc_rotation_data[misc_id];
    rotation_state->target_facing = cursor.placement->facing * 4;
    rotation_state->delay = 0;
    rotation_state->rotating = 0;
    battle_unit_place_in_new_location(misc_id, &tile, cursor.placement->facing);
    battle_unit_set_facing(misc_id, cursor.placement->facing * 4);
}
