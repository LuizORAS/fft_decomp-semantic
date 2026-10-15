#include "fft/battle.h"
#include "psx/types.h"

/* Walk phases 1-15 (odd; centre to edge). At the current tile's exit edge, a step across a gap
 * (step bits 0-1) starts a jump step. Otherwise it compares the two edge heights, in half-heights:
 * each is the top of the unit standing there (the one the walker stands on, or the one it steps
 * onto) when that unit's height is available, else the tile edge; with no unit under the walker,
 * the current edge also adds its water standing offset. Up to +1 starts the walk to the next centre
 * (phase 6, or 0xe at climb speed when the step's destination-climb bit is set), up to +7 a climb
 * hop (battle_move_start_unit_climb_hop_step), more a climb jump
 * (battle_move_start_unit_climb_jump_step). A step down is always a walk; falling comes from
 * battle_move_apply_unit_step_velocity. The destination's water offset is added to the current
 * height (QUIRKS.md). */
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
                    + (g_battle_move_current_tile->depth_half_height.value & MAP_TILE_HALF_HEIGHT_MASK)
                        * unit->current_edge_height;
            } else {
                g_battle_move_current_edge_height = g_battle_move_tile_occupant_height.total_height;
            }
        } else {
            g_battle_move_current_edge_height = g_battle_move_current_tile->height * 2
                + (g_battle_move_current_tile->depth_half_height.value & MAP_TILE_HALF_HEIGHT_MASK)
                    * unit->current_edge_height;
            g_battle_move_current_edge_height
                += battle_move_get_water_standing_offset(unit, g_battle_move_current_tile);
        }

        if (g_battle_move_step_value.bits.mount) {
            g_battle_move_tile_occupant
                = battle_unit_find_any_misc_data_at_map_coords(unit->movement.bytes.destination_x,
                    unit->movement.bytes.destination_y, unit->movement.bytes.destination_z);
            unit->standing_on_unit_id_plus_one = g_battle_move_tile_occupant->battle_data->misc_unit_id + 1;
            battle_unit_calculate_height_data(
                &g_battle_move_tile_occupant_height, g_battle_move_tile_occupant->battle_data->misc_unit_id);
            if (g_battle_move_tile_occupant_height.unit_flags & BATTLE_UNIT_HEIGHT_UNAVAILABLE) {
                g_battle_move_destination_edge_height = g_battle_move_destination_tile->height * 2
                    + (g_battle_move_destination_tile->depth_half_height.value & MAP_TILE_HALF_HEIGHT_MASK)
                        * unit->destination_edge_height;
            } else {
                g_battle_move_destination_edge_height = g_battle_move_tile_occupant_height.total_height;
            }
        } else {
            unit->standing_on_unit_id_plus_one = 0;
            g_battle_move_destination_edge_height = g_battle_move_destination_tile->height * 2
                + (g_battle_move_destination_tile->depth_half_height.value & MAP_TILE_HALF_HEIGHT_MASK)
                    * unit->destination_edge_height;
            g_battle_move_current_edge_height
                += battle_move_get_water_standing_offset(unit, g_battle_move_destination_tile);
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
