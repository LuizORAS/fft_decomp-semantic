#include "fft/battle.h"
#include "psx/types.h"

/* CallFunction 0x0f and 0x10: place the misc unit of battle id 3 on the tile and
 * facing of battle id 1, or that of battle id 4 on battle id 2, and cancel any
 * rotation in progress. Nothing happens when the warped battle id has no misc
 * unit.
 *
 * The parameter arrives as the CallFunction selector and is reused for the misc
 * id. */
void battle_script_warp_unit_display_to_paired_unit(s32 misc_id) {
    battle_stats_t* unit;
    unit_animation_state_t* state;
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
        g_battle_script_warp_coords.elevation = unit->position.higher_elevation;
        g_battle_script_warp_coords.y = unit->position.y;
        facing = unit->position.facing << 2;
        state = &g_battle_unit_misc_rotation_data[misc_id];
        state->delay = 0;
        state->rotating = 0;
        state->target_facing = facing;
        battle_unit_place_in_new_location(misc_id, &g_battle_script_warp_coords, unit->position.facing);
        battle_unit_set_facing(misc_id, unit->position.facing << 2);
    }
}
