#include "fft/battle.h"
#include "psx/types.h"

/* Per-frame movement step for a unit walking between two tiles.
 *
 * Once the unit reaches the exit edge of the tile it occupies, the step is
 * re-planned: a jumping unit (step value bits 0-1) restarts a jump step, and
 * otherwise the edge heights of the current and destination tiles are recomputed
 * from the tile height/depth pair scaled by the unit's per-step scales, taking
 * a mount's walking height into account when one is present and float/fly slope
 * otherwise.  A height difference of at most one step starts an ordinary step
 * (the WALK_TO_CENTRE phase) or a climb-speed step (CLIMB_TO_CENTRE); up to seven
 * starts battle_move_start_unit_climb_hop_step, and anything higher
 * battle_move_start_unit_climb_jump_step. */
void battle_move_update_walking_step_at_tile_edge(battle_unit_misc_data_t* unit) {
    s32 direction;
    u8 rider;

    direction = battle_move_get_direction(unit);
    battle_move_apply_unit_step_velocity(unit);
    if (g_battle_game_state != BATTLE_GAME_STATE_EVENT) {
        battle_move_displace_overlapping_unit(unit, direction);
    }
    if (battle_move_has_reached_current_tile_exit_edge(direction, unit) == 0) {
        return;
    }
    battle_move_snap_axis_to_current_tile_exit_edge(direction, unit);
    if (g_battle_move_step_value.bits.length == 0) {
        rider = unit->standing_on_unit_id_plus_one;
        if (rider != 0) {
            battle_unit_calculate_height_data(&g_battle_move_tile_occupant_height, rider - 1);
            if (g_battle_move_tile_occupant_height.unit_flags & BATTLE_UNIT_HEIGHT_UNAVAILABLE) {
                g_battle_move_current_edge_height = g_battle_move_current_tile->height * 2
                    + g_battle_move_current_tile->depth_half_height.bits.half_height * unit->current_edge_height;
            } else {
                g_battle_move_current_edge_height = g_battle_move_tile_occupant_height.total_height;
            }
        } else {
            g_battle_move_current_edge_height = g_battle_move_current_tile->height * 2
                + g_battle_move_current_tile->depth_half_height.bits.half_height * unit->current_edge_height;
            g_battle_move_current_edge_height += battle_move_calculate_water_lift(unit, g_battle_move_current_tile);
        }

        if (g_battle_move_step_value.bits.mount) {
            g_battle_move_tile_occupant = battle_unit_find_any_misc_data_at_map_coords(
                unit->movement.destination_x, unit->movement.destination_y, unit->movement.destination_z);
            unit->standing_on_unit_id_plus_one = g_battle_move_tile_occupant->battle_data->misc_unit_id + 1;
            battle_unit_calculate_height_data(
                &g_battle_move_tile_occupant_height, g_battle_move_tile_occupant->battle_data->misc_unit_id);
            if (g_battle_move_tile_occupant_height.unit_flags & BATTLE_UNIT_HEIGHT_UNAVAILABLE) {
                g_battle_move_destination_edge_height = g_battle_move_destination_tile->height * 2
                    + g_battle_move_destination_tile->depth_half_height.bits.half_height
                        * unit->destination_edge_height;
            } else {
                g_battle_move_destination_edge_height = g_battle_move_tile_occupant_height.total_height;
            }
        } else {
            unit->standing_on_unit_id_plus_one = 0;
            g_battle_move_destination_edge_height = g_battle_move_destination_tile->height * 2
                + g_battle_move_destination_tile->depth_half_height.bits.half_height * unit->destination_edge_height;
            g_battle_move_current_edge_height += battle_move_calculate_water_lift(unit, g_battle_move_destination_tile);
        }

        if (g_battle_move_current_edge_height + 1 >= g_battle_move_destination_edge_height) {
            battle_unit_dismount_rider_and_update_display(unit);
            switch (g_battle_move_step_value.bits.climb) {
            case 0:
                battle_move_start_unit_step(unit, g_battle_move_current_tile, g_battle_move_destination_tile);
                unit->step_phase = BATTLE_MOVE_STEP_PHASE_WALK_TO_CENTRE_NORTH;
                return;
            case 1:
                battle_move_start_unit_step_at_climb_speed(
                    unit, g_battle_move_current_tile, g_battle_move_destination_tile);
                unit->step_phase = BATTLE_MOVE_STEP_PHASE_CLIMB_TO_CENTRE_NORTH;
                return;
            }
            return;
        }

        if (g_battle_move_current_edge_height + 7 >= g_battle_move_destination_edge_height) {
            battle_unit_dismount_rider(unit);
            unit->step_speed = 0x2000;
            battle_move_start_unit_climb_hop_step(unit, g_battle_move_current_tile, g_battle_move_destination_tile);
            unit->standing_on_unit_id_plus_one = 0;
            return;
        }
        unit->step_speed = 0x2000;
        battle_move_start_unit_climb_jump_step(unit, g_battle_move_current_tile, g_battle_move_destination_tile);
        unit->standing_on_unit_id_plus_one = 0;
    } else {
        unit->step_speed = 0x2000;
        battle_move_start_unit_jump_step(unit, g_battle_move_current_tile, g_battle_move_destination_tile);
        unit->standing_on_unit_id_plus_one = 0;
    }
}
