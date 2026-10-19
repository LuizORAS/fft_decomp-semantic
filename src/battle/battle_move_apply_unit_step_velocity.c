#include "fft/battle.h"
#include "fft/battle_map.h"
#include "fft/battle_move.h"
#include "psx/types.h"

/* Move the unit by its velocity for one frame: take the destination's layer on reaching its tile
 * (0x1c000 in Q12, 28 world units, per tile), apply gravity in jumps and falls, and on landing
 * start the next half of the step for its phase; a unit 42 (0x2a) or more above the ground starts
 * falling. Then redraw its shadow and carry a rider with its mount, 10 units above it.
 *
 * Per stage: walking and climbing snap onto the ground at or below it, and within 6 units above it,
 * and rebuild the step delta (climbing first raises step_speed by g_battle_move_jump_gravity, up to
 * 0x7fff); higher up they gain gravity, and 0x2a units up (above the destination's ground for the
 * to-centre halves) turns the step into a jump, with the fall animation outside action execution.
 * Jumps and float jumps gain gravity and stop at the ground on the way down. A float snaps at the
 * ground and rebuilds its delta; above it the fall speed gains gravity while under 0x2000. A flight
 * rebuilds its velocity for the run of contiguous steps when it reaches the ground.
 *
 * The first switch repeats the layer test that the statement before it has just made false, so its
 * bodies never run. */
void battle_move_apply_unit_step_velocity(battle_unit_t* unit) {
    s16 tile[2];
    u8 layer;
    s16 ground_height;
    s16 destination_height;
    map_tile_t* map_tile;
    battle_unit_t* rider;
    s32 depth;

    tile[0] = (unit->real.vx + unit->velocity.vx) / (BATTLE_MAP_TILE_SIZE * ONE);
    tile[1] = (unit->real.vz + unit->velocity.vz) / (BATTLE_MAP_TILE_SIZE * ONE);
    layer = unit->map_z;
    if (tile[0] == unit->movement.destination_x && tile[1] == unit->movement.destination_y
        && unit->map_z != unit->movement.destination_z) {
        layer = unit->map_z = unit->movement.destination_z;
    }
    switch (unit->step_phase) {
    case BATTLE_MOVE_STEP_PHASE_WALK_TO_CENTRE_SOUTH:
    case BATTLE_MOVE_STEP_PHASE_WALK_TO_CENTRE_EAST:
    case BATTLE_MOVE_STEP_PHASE_WALK_TO_CENTRE_NORTH:
    case BATTLE_MOVE_STEP_PHASE_WALK_TO_CENTRE_WEST:
        if (tile[0] == unit->movement.destination_x && tile[1] == unit->movement.destination_y
            && unit->map_z != unit->movement.destination_z) {
            layer = unit->map_z = unit->movement.destination_z;
            battle_move_set_unit_step_delta_edge_to_center(unit, &unit->current_path_step,
                battle_map_get_tile_data_pointer(tile[0], tile[1], layer), unit->destination_edge_height);
            battle_move_interpolate_partial(unit, unit->walk_speed);
        }
        break;
    case BATTLE_MOVE_STEP_PHASE_CLIMB_TO_CENTRE_SOUTH:
    case BATTLE_MOVE_STEP_PHASE_CLIMB_TO_CENTRE_EAST:
    case BATTLE_MOVE_STEP_PHASE_CLIMB_TO_CENTRE_NORTH:
    case BATTLE_MOVE_STEP_PHASE_CLIMB_TO_CENTRE_WEST:
        if (tile[0] == unit->movement.destination_x && tile[1] == unit->movement.destination_y
            && unit->map_z != unit->movement.destination_z) {
            layer = unit->map_z = unit->movement.destination_z;
            battle_move_set_unit_step_delta_edge_to_center(unit, &unit->current_path_step,
                battle_map_get_tile_data_pointer(tile[0], tile[1], layer), unit->destination_edge_height);
            battle_move_interpolate_partial(unit, unit->step_speed);
        }
        break;
    }
    unit->real.vx += unit->velocity.vx;
    unit->real.vz += unit->velocity.vz;
    unit->world_coords.vx = unit->real.vx / ONE;
    unit->world_coords.vz = unit->real.vz / ONE;
    ground_height = battle_gfx_calculate_world_height_from_world_coords(unit);
    destination_height = battle_gfx_calculate_world_height_from_move_destination(unit);
    switch (unit->step_phase) {
    case BATTLE_MOVE_STEP_PHASE_WALK_TO_EDGE_SOUTH:
    case BATTLE_MOVE_STEP_PHASE_WALK_TO_EDGE_EAST:
    case BATTLE_MOVE_STEP_PHASE_WALK_TO_EDGE_NORTH:
    case BATTLE_MOVE_STEP_PHASE_WALK_TO_EDGE_WEST:
        unit->real.vy += unit->velocity.vy;
        map_tile = battle_map_get_tile_data_pointer(tile[0], tile[1], layer);
        if ((-unit->real.vy / ONE) <= -ground_height) {
            unit->real.vy = ground_height * ONE;
            battle_move_set_unit_step_delta_center_to_edge(
                unit, &unit->current_path_step, map_tile, unit->current_edge_height);
            battle_move_interpolate_partial(unit, unit->walk_speed);
        } else {
            if ((-unit->real.vy / ONE) < -ground_height + 6) {
                unit->real.vy = ground_height * ONE;
            } else if (unit->velocity.vy <= 0x7FFF) {
                unit->velocity.vy += g_battle_move_jump_gravity;
            }
            if (-unit->real.vy / ONE >= -ground_height + 0x2A) {
                unit->step_phase = BATTLE_MOVE_STEP_PHASE_JUMP_ASCENT_NORTH;
                if (g_battle_game_state != BATTLE_GAME_STATE_ACTION_EXECUTE) {
                    battle_unit_store_animation_facing(BATTLE_UNIT_ANIMATION_FALL, unit->facing, unit);
                }
            }
        }
        unit->shadow_dirty |= 1;
        break;
    case BATTLE_MOVE_STEP_PHASE_CLIMB_TO_EDGE_SOUTH:
    case BATTLE_MOVE_STEP_PHASE_CLIMB_TO_EDGE_EAST:
    case BATTLE_MOVE_STEP_PHASE_CLIMB_TO_EDGE_NORTH:
    case BATTLE_MOVE_STEP_PHASE_CLIMB_TO_EDGE_WEST:
        unit->real.vy += unit->velocity.vy;
        map_tile = battle_map_get_tile_data_pointer(tile[0], tile[1], layer);
        if ((-unit->real.vy / ONE) <= -ground_height) {
            unit->real.vy = ground_height * ONE;
            if (unit->step_speed <= 0x7FFF) {
                unit->step_speed += g_battle_move_jump_gravity;
            }
            battle_move_set_unit_step_delta_center_to_edge(
                unit, &unit->current_path_step, map_tile, unit->current_edge_height);
            battle_move_interpolate_partial(unit, unit->step_speed);
        } else {
            if ((-unit->real.vy / ONE) < -ground_height + 6) {
                unit->real.vy = ground_height * ONE;
            } else if (unit->velocity.vy <= 0x7FFF) {
                unit->velocity.vy += g_battle_move_jump_gravity;
            }
            if (-unit->real.vy / ONE >= -ground_height + 0x2A) {
                unit->step_phase = BATTLE_MOVE_STEP_PHASE_JUMP_ASCENT_NORTH;
                if (g_battle_game_state != BATTLE_GAME_STATE_ACTION_EXECUTE) {
                    battle_unit_store_animation_facing(BATTLE_UNIT_ANIMATION_FALL, unit->facing, unit);
                }
            }
        }
        unit->shadow_dirty |= 1;
        break;
    case BATTLE_MOVE_STEP_PHASE_WALK_TO_CENTRE_SOUTH:
    case BATTLE_MOVE_STEP_PHASE_WALK_TO_CENTRE_EAST:
    case BATTLE_MOVE_STEP_PHASE_WALK_TO_CENTRE_NORTH:
    case BATTLE_MOVE_STEP_PHASE_WALK_TO_CENTRE_WEST:
        unit->real.vy += unit->velocity.vy;
        map_tile = battle_map_get_tile_data_pointer(tile[0], tile[1], layer);
        if (-unit->real.vy / ONE <= -ground_height) {
            unit->real.vy = ground_height * ONE;
            battle_move_set_unit_step_delta_edge_to_center(
                unit, &unit->current_path_step, map_tile, unit->destination_edge_height);
            battle_move_interpolate_partial(unit, unit->walk_speed);
        }
        if (-ground_height < (-unit->real.vy / ONE)) {
            if ((-unit->real.vy / ONE) < -ground_height + 6) {
                unit->real.vy = ground_height * ONE;
            } else if (unit->velocity.vy <= 0x7FFF) {
                unit->velocity.vy += g_battle_move_jump_gravity;
            }
            if (-unit->real.vy / ONE >= -destination_height + 0x2A) {
                unit->step_phase = BATTLE_MOVE_STEP_PHASE_JUMP_DESCENT_NORTH;
                if (g_battle_game_state != BATTLE_GAME_STATE_ACTION_EXECUTE) {
                    battle_unit_store_animation_facing(BATTLE_UNIT_ANIMATION_FALL, unit->facing, unit);
                }
            }
        }
        unit->shadow_dirty |= 1;
        break;
    case BATTLE_MOVE_STEP_PHASE_CLIMB_TO_CENTRE_SOUTH:
    case BATTLE_MOVE_STEP_PHASE_CLIMB_TO_CENTRE_EAST:
    case BATTLE_MOVE_STEP_PHASE_CLIMB_TO_CENTRE_NORTH:
    case BATTLE_MOVE_STEP_PHASE_CLIMB_TO_CENTRE_WEST:
        unit->real.vy += unit->velocity.vy;
        map_tile = battle_map_get_tile_data_pointer(tile[0], tile[1], layer);
        if (-unit->real.vy / ONE <= -ground_height) {
            unit->real.vy = ground_height * ONE;
            if (unit->step_speed <= 0x7FFF) {
                unit->step_speed += g_battle_move_jump_gravity;
            }
            battle_move_set_unit_step_delta_edge_to_center(
                unit, &unit->current_path_step, map_tile, unit->destination_edge_height);
            battle_move_interpolate_partial(unit, unit->step_speed);
        }
        if ((-unit->real.vy / ONE) > -ground_height) {
            if ((-unit->real.vy / ONE) < -ground_height + 6) {
                unit->real.vy = ground_height * ONE;
            } else if (unit->velocity.vy <= 0x7FFF) {
                unit->velocity.vy += g_battle_move_jump_gravity;
            }
            if (-unit->real.vy / ONE >= -destination_height + 0x2A) {
                unit->step_phase = BATTLE_MOVE_STEP_PHASE_JUMP_DESCENT_NORTH;
                if (g_battle_game_state != BATTLE_GAME_STATE_ACTION_EXECUTE) {
                    battle_unit_store_animation_facing(BATTLE_UNIT_ANIMATION_FALL, unit->facing, unit);
                }
            }
        }
        unit->shadow_dirty |= 1;
        break;
    case BATTLE_MOVE_STEP_PHASE_JUMP_ASCENT_SOUTH:
    case BATTLE_MOVE_STEP_PHASE_JUMP_DESCENT_SOUTH:
    case BATTLE_MOVE_STEP_PHASE_JUMP_ASCENT_EAST:
    case BATTLE_MOVE_STEP_PHASE_JUMP_DESCENT_EAST:
    case BATTLE_MOVE_STEP_PHASE_JUMP_ASCENT_NORTH:
    case BATTLE_MOVE_STEP_PHASE_JUMP_DESCENT_NORTH:
    case BATTLE_MOVE_STEP_PHASE_JUMP_ASCENT_WEST:
    case BATTLE_MOVE_STEP_PHASE_JUMP_DESCENT_WEST:
    case BATTLE_MOVE_STEP_PHASE_FLOAT_JUMP_SOUTH:
    case BATTLE_MOVE_STEP_PHASE_FLOAT_JUMP_EAST:
    case BATTLE_MOVE_STEP_PHASE_FLOAT_JUMP_NORTH:
    case BATTLE_MOVE_STEP_PHASE_FLOAT_JUMP_WEST:
        if (unit->velocity.vy <= 0x7FFF) {
            unit->velocity.vy += g_battle_move_jump_gravity;
        }
        unit->real.vy += unit->velocity.vy;
        if (unit->velocity.vy > 0) {
            if (-unit->real.vy / ONE < -ground_height) {
                unit->real.vy = ground_height * ONE;
            }
        }
        unit->shadow_dirty |= 1;
        break;
    case BATTLE_MOVE_STEP_PHASE_FLOAT_TO_EDGE_SOUTH:
    case BATTLE_MOVE_STEP_PHASE_FLOAT_TO_EDGE_EAST:
    case BATTLE_MOVE_STEP_PHASE_FLOAT_TO_EDGE_NORTH:
    case BATTLE_MOVE_STEP_PHASE_FLOAT_TO_EDGE_WEST:
        unit->real.vy += unit->velocity.vy;
        map_tile = battle_map_get_tile_data_pointer(tile[0], tile[1], layer);
        depth = -unit->real.vy;
        if (depth / ONE <= -ground_height) {
            unit->real.vy = ground_height * ONE;
            battle_move_set_float_step_delta_center_to_edge(
                unit, &unit->current_path_step, map_tile, unit->destination_edge_height);
            battle_move_interpolate_partial(unit, unit->walk_speed);
        } else {
            /* Shared with the next case: a duplicated branch does not merge. */
            goto accelerate;
        }
        unit->shadow_dirty |= 1;
        break;
    case BATTLE_MOVE_STEP_PHASE_FLOAT_TO_CENTRE_SOUTH:
    case BATTLE_MOVE_STEP_PHASE_FLOAT_TO_CENTRE_EAST:
    case BATTLE_MOVE_STEP_PHASE_FLOAT_TO_CENTRE_NORTH:
    case BATTLE_MOVE_STEP_PHASE_FLOAT_TO_CENTRE_WEST:
        unit->real.vy += unit->velocity.vy;
        map_tile = battle_map_get_tile_data_pointer(tile[0], tile[1], layer);
        depth = -unit->real.vy;
        if (depth / ONE <= -ground_height) {
            unit->real.vy = ground_height * ONE;
            battle_move_set_float_step_delta_edge_to_center(
                unit, &unit->current_path_step, map_tile, unit->destination_edge_height);
            battle_move_interpolate_partial(unit, unit->walk_speed);
        } else {
        accelerate:
            /* Keeps depth live here, so it takes a0 and reorg leaves `move a0,s0` out of both landing tests. */
            __asm__ volatile("" : : "r"(depth));
            if (unit->velocity.vy < 0x2000) {
                unit->velocity.vy += g_battle_move_jump_gravity;
            }
        }
        unit->shadow_dirty |= 1;
        break;
    case BATTLE_MOVE_STEP_PHASE_FLY_TO_EDGE_SOUTH:
    case BATTLE_MOVE_STEP_PHASE_FLY_TO_EDGE_EAST:
    case BATTLE_MOVE_STEP_PHASE_FLY_TO_EDGE_NORTH:
    case BATTLE_MOVE_STEP_PHASE_FLY_TO_EDGE_WEST:
        unit->real.vy += unit->velocity.vy;
        battle_map_get_tile_data_pointer(tile[0], tile[1], layer);
        if (-unit->real.vy / ONE <= -ground_height) {
            unit->real.vy = ground_height * ONE;
            battle_move_set_velocity_for_contiguous_clamped_steps(unit, &unit->path, &unit->current_path_step);
            battle_move_interpolate_partial(unit, unit->walk_speed);
        }
        unit->shadow_dirty |= 1;
        break;
    case BATTLE_MOVE_STEP_PHASE_FLY_TO_CENTRE_SOUTH:
    case BATTLE_MOVE_STEP_PHASE_FLY_TO_CENTRE_EAST:
    case BATTLE_MOVE_STEP_PHASE_FLY_TO_CENTRE_NORTH:
    case BATTLE_MOVE_STEP_PHASE_FLY_TO_CENTRE_WEST:
        unit->real.vy += unit->velocity.vy;
        battle_map_get_tile_data_pointer(tile[0], tile[1], layer);
        depth = -unit->real.vy;
        if (depth / ONE <= -ground_height) {
            unit->real.vy = ground_height * ONE;
            battle_move_set_velocity_for_contiguous_steps_with_final_tile_height(
                unit, &unit->path, &unit->current_path_step);
            battle_move_interpolate_partial(unit, unit->walk_speed);
        }
        unit->shadow_dirty |= 1;
        break;
    }
    battle_unit_set_world_coords_from_real_coords(unit);
    if (unit->mount_state == BATTLE_UNIT_MOUNT_STATE_MOUNT) {
        rider = battle_unit_get_by_runtime_id(unit->mount_partner_runtime_id);
        if (rider != 0) {
            rider->world_coords = unit->world_coords;
            rider->world_coords.vy -= 10;
            rider->real = unit->real;
            rider->real.vy -= 0xA000;
        }
    }
}
