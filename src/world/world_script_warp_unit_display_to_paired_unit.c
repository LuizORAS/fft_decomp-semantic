#include "fft/battle_unit.h"
#include "fft/world.h"
#include "psx/types.h"

/*
 * CallFunction helper: move the display of battle unit 3 onto battle unit
 * 1's tile and facing, or (for any other argument) unit 4 onto unit 2's.
 * Nothing happens when the moved unit has no unit id.
 *
 * Declared to return a value but none is set.
 */
s32 world_script_warp_unit_display_to_paired_unit(s32 runtime_id) {
    unit_stats_t* stats;
    unit_animation_state_t* state;
    s32 facing;
    s32 battle_id;

    if (runtime_id == EVENT_CALL_FUNCTION_WARP_UNIT_4_DISPLAY_TO_UNIT_2_POSITION_AND_FACING) {
        stats = battle_unit_get_stats_from_battle_id(1);
        battle_id = 3;
    } else {
        stats = battle_unit_get_stats_from_battle_id(2);
        battle_id = 4;
    }
    runtime_id = battle_unit_get_runtime_id_by_battle_id(battle_id);
    if (runtime_id != -1) {
        g_world_script_warp_unit_coords.x = stats->x;
        g_world_script_warp_unit_coords.layer = stats->position.higher_elevation;
        g_world_script_warp_unit_coords.y = stats->position.y;
        facing = stats->position.facing << 2;
        state = &g_unit_animation_states[runtime_id];
        state->delay = 0;
        state->rotating = 0;
        state->target_facing = facing;
        battle_unit_place_in_new_location(runtime_id, &g_world_script_warp_unit_coords, stats->position.facing);
        battle_unit_set_facing(runtime_id, stats->position.facing << 2);
    }
}
