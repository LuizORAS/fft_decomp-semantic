#include "fft/battle.h"
#include "psx/types.h"

/* Three-halfword map coordinate (x, elevation, y) staged for
 * battle_unit_place_in_new_location; no header declares it. The WORLD copy
 * stages the same triple in g_world_script_warp_unit_coords. */
extern world_unit_coordinates_t g_battle_script_warp_coords;

/* CallFunction 0x0f and 0x10: place the misc unit of battle id 3 on the tile and
 * facing of battle id 1, or that of battle id 4 on battle id 2, and cancel any
 * rotation in progress. Nothing happens when the warped battle id has no misc
 * unit.
 *
 * The parameter arrives as the CallFunction selector and is reused for the misc
 * id. */
void battle_script_warp_unit_display_to_paired_unit(s32 misc_id) {
    battle_stats_t* unit;
    world_unit_animation_state_t* state;
    s32 facing;
    s32 battle_id;

    if (misc_id == EVENT_CALL_FUNCTION_WARP_UNIT_4_DISPLAY_TO_UNIT_2_POSITION_AND_FACING) {
        unit = battle_unit_get_stats_from_battle_id(1);
        battle_id = 3;
    } else {
        unit = battle_unit_get_stats_from_battle_id(2);
        battle_id = 4;
    }
    misc_id = battle_unit_get_misc_id_by_battle_id(battle_id);
    if (misc_id != -1) {
        g_battle_script_warp_coords.x = unit->x;
        g_battle_script_warp_coords.elevation = unit->position.bits.higher_elevation;
        g_battle_script_warp_coords.y = unit->position.bits.y;
        facing = (unit->position.raw >> 6) & 0x3C;
        state = &g_battle_unit_misc_rotation_data[misc_id];
        state->delay = 0;
        state->rotating = 0;
        state->target_facing = facing;
        battle_unit_place_in_new_location(misc_id, (s16*)&g_battle_script_warp_coords, unit->position.bits.facing);
        battle_unit_set_facing(misc_id, (unit->position.raw >> 6) & 0x3C);
    }
}
