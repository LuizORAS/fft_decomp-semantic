#include "fft/battle.h"
#include "psx/types.h"

/* battle_move_start_unit_step / _at_climb_speed and battle_move_start_unit_jump_step
 * are declared in fft/battle.h with fewer (or differently typed) parameters
 * than this call site passes.  The target hands all three the unit plus both
 * tile pointers, so the calls go through a cast. */
#define START_STEP(f) ((void (*)(battle_unit_misc_data_t*, const map_tile_t*, const map_tile_t*))(f))

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
    if ((g_battle_move_step_value & 3) == 0) {
        rider = unit->current_unit_id_plus_one;
        if (rider != 0) {
            battle_calculate_unit_height_data(&g_battle_move_tile_occupant_height, rider - 1);
            if (g_battle_move_tile_occupant_height.unit_flags & 0x80) {
                g_battle_move_current_edge_height = g_battle_move_current_tile->height * 2
                    + (g_battle_move_current_tile->depth_half_height & MAP_TILE_HALF_HEIGHT_MASK)
                        * unit->current_edge_height;
            } else {
                g_battle_move_current_edge_height = g_battle_move_tile_occupant_height.total_height;
            }
        } else {
            g_battle_move_current_edge_height = g_battle_move_current_tile->height * 2
                + (g_battle_move_current_tile->depth_half_height & MAP_TILE_HALF_HEIGHT_MASK)
                    * unit->current_edge_height;
            g_battle_move_current_edge_height
                += battle_move_get_water_standing_offset(unit, g_battle_move_current_tile);
        }

        if (g_battle_move_step_value & 0x10) {
            g_battle_move_tile_occupant
                = battle_unit_find_any_misc_data_at_map_coords(unit->movement.bytes.destination_x,
                    unit->movement.bytes.destination_y, unit->movement.bytes.destination_z);
            unit->current_unit_id_plus_one = g_battle_move_tile_occupant->battle_data->misc_unit_id + 1;
            battle_calculate_unit_height_data(
                &g_battle_move_tile_occupant_height, g_battle_move_tile_occupant->battle_data->misc_unit_id);
            if (g_battle_move_tile_occupant_height.unit_flags & 0x80) {
                g_battle_move_destination_edge_height = g_battle_move_destination_tile->height * 2
                    + (g_battle_move_destination_tile->depth_half_height & MAP_TILE_HALF_HEIGHT_MASK)
                        * unit->destination_edge_height;
            } else {
                g_battle_move_destination_edge_height = g_battle_move_tile_occupant_height.total_height;
            }
        } else {
            unit->current_unit_id_plus_one = 0;
            g_battle_move_destination_edge_height = g_battle_move_destination_tile->height * 2
                + (g_battle_move_destination_tile->depth_half_height & MAP_TILE_HALF_HEIGHT_MASK)
                    * unit->destination_edge_height;
            g_battle_move_current_edge_height
                += battle_move_get_water_standing_offset(unit, g_battle_move_destination_tile);
        }

        if (g_battle_move_current_edge_height + 1 >= (s32)g_battle_move_destination_edge_height) {
            battle_unit_dismount_rider_and_update_display(unit);
            switch ((g_battle_move_step_value >> 3) & 1) {
            case 0:
                START_STEP(battle_move_start_unit_step)
                (unit, g_battle_move_current_tile, g_battle_move_destination_tile);
                unit->step_phase = 6;
                return;
            case 1:
                START_STEP(battle_move_start_unit_step_at_climb_speed)
                (unit, g_battle_move_current_tile, g_battle_move_destination_tile);
                unit->step_phase = 0xe;
                return;
            }
            return;
        }

        if (g_battle_move_current_edge_height + 7 >= (s32)g_battle_move_destination_edge_height) {
            battle_unit_dismount_rider(unit);
            unit->step_speed = 0x2000;
            battle_move_start_unit_climb_hop_step(unit, g_battle_move_current_tile, g_battle_move_destination_tile);
            unit->current_unit_id_plus_one = 0;
            return;
        }
        unit->step_speed = 0x2000;
        battle_move_start_unit_climb_jump_step(unit, g_battle_move_current_tile, g_battle_move_destination_tile);
        unit->current_unit_id_plus_one = 0;
    } else {
        unit->step_speed = 0x2000;
        START_STEP(battle_move_start_unit_jump_step)
        (unit, g_battle_move_current_tile, g_battle_move_destination_tile);
        unit->current_unit_id_plus_one = 0;
    }
}
