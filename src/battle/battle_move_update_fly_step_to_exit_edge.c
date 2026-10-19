#include "fft/battle_move.h"
#include "psx/types.h"

/* Fly phases 0x22-0x28 (even; set by battle_move_start_fly_step): move, and at the current
 * tile's exit edge snap to it, work out both edge heights, clear the unit it stood on and set
 * the velocity for the run of following steps in the same direction at one flying height, aimed
 * at the destination's surface on the path's last step
 * (battle_move_set_velocity_for_contiguous_steps_with_final_tile_height, scaled by the walk
 * speed); then enter the vertical fly phase 0x27 (BATTLE_MOVE_STEP_PHASE_FLY_TO_CENTRE_NORTH,
 * whatever the direction). */
void battle_move_update_fly_step_to_exit_edge(battle_unit_t* unit) {
    s32 direction;

    direction = battle_move_get_direction(unit);
    battle_move_apply_unit_step_velocity(unit);
    if (battle_move_has_reached_current_tile_exit_edge(direction, unit) != 0) {
        battle_move_snap_axis_to_current_tile_exit_edge(direction, unit);
        g_battle_move_current_edge_height = g_battle_move_current_tile->height * 2
            + g_battle_move_current_tile->depth_half_height.half_height * unit->current_edge_height;
        unit->standing_on_battle_id_plus_one = 0;
        g_battle_move_destination_edge_height = g_battle_move_destination_tile->height * 2
            + g_battle_move_destination_tile->depth_half_height.half_height * unit->destination_edge_height;
        battle_move_set_velocity_for_contiguous_steps_with_final_tile_height(
            unit, &unit->path, &unit->current_path_step);
        battle_move_interpolate_partial(unit, unit->walk_speed);
        unit->step_phase = BATTLE_MOVE_STEP_PHASE_FLY_TO_CENTRE_NORTH;
    }
}
