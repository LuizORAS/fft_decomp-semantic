#include "fft/battle.h"
#include "psx/types.h"

/*
 * Advance a knocked-back or path-moving unit by one frame.
 *
 * While a step is in progress (step_phase non-zero) the walk speed
 * decays by a quarter towards 0x1000 and the step phase handler runs. Once
 * no step is active, the next path byte starts a new step, or the mount
 * animation coordinates are stored when the path is exhausted.
 */
void battle_move_update_knockback_step(battle_unit_misc_data_t* unit) {
    u32 offset;
    s32 speed;

    battle_move_get_current_and_destination_tiles(unit, &g_battle_move_current_tile, &g_battle_move_destination_tile);
    g_battle_move_step_value = unit->current_path_step;
    if (unit->step_phase != BATTLE_MOVE_STEP_PHASE_NONE) {
        speed = unit->walk_speed.word;
        unit->walk_speed.word = (speed > ONE) ? speed / 4 * 3 : ONE;
        switch (unit->step_phase) {
        case BATTLE_MOVE_STEP_PHASE_WALK_TO_CENTRE_SOUTH:
        case BATTLE_MOVE_STEP_PHASE_WALK_TO_CENTRE_EAST:
        case BATTLE_MOVE_STEP_PHASE_WALK_TO_CENTRE_NORTH:
        case BATTLE_MOVE_STEP_PHASE_WALK_TO_CENTRE_WEST:
            battle_move_update_unit_step_to_destination_tile_center(unit);
            break;
        case BATTLE_MOVE_STEP_PHASE_WALK_TO_EDGE_SOUTH:
        case BATTLE_MOVE_STEP_PHASE_WALK_TO_EDGE_EAST:
        case BATTLE_MOVE_STEP_PHASE_WALK_TO_EDGE_NORTH:
        case BATTLE_MOVE_STEP_PHASE_WALK_TO_EDGE_WEST:
            battle_move_update_walking_step_at_tile_edge(unit);
            break;
        case BATTLE_MOVE_STEP_PHASE_JUMP_LANDING_SOUTH:
        case BATTLE_MOVE_STEP_PHASE_JUMP_LANDING_EAST:
        case BATTLE_MOVE_STEP_PHASE_JUMP_LANDING_NORTH:
        case BATTLE_MOVE_STEP_PHASE_JUMP_LANDING_WEST:
            battle_move_update_knockback_after_animation(unit);
            break;
        case BATTLE_MOVE_STEP_PHASE_JUMP_ASCENT_SOUTH:
        case BATTLE_MOVE_STEP_PHASE_JUMP_ASCENT_EAST:
        case BATTLE_MOVE_STEP_PHASE_JUMP_ASCENT_NORTH:
        case BATTLE_MOVE_STEP_PHASE_JUMP_ASCENT_WEST:
            battle_move_update_airborne_ascent_phase(unit);
            /* fallthrough */
        case BATTLE_MOVE_STEP_PHASE_JUMP_DESCENT_SOUTH:
        case BATTLE_MOVE_STEP_PHASE_JUMP_DESCENT_EAST:
        case BATTLE_MOVE_STEP_PHASE_JUMP_DESCENT_NORTH:
        case BATTLE_MOVE_STEP_PHASE_JUMP_DESCENT_WEST:
            battle_move_finish_unit_step_at_tile_edge(unit);
            break;
        }
        if (unit->step_phase != BATTLE_MOVE_STEP_PHASE_NONE) {
            return;
        }
    }
    if (unit->path.count != 0) {
        offset = unit->movement_path_offset;
        if (offset < unit->path.count) {
            unit->movement_path_offset = offset + 1;
            unit->current_path_step = g_battle_move_step_value = unit->path.data.steps[offset];
            battle_move_set_unit_step_slope_scales(unit);
            unit->step_speed = 0x2000;
            battle_move_start_unit_step(unit, g_battle_move_current_tile, g_battle_move_destination_tile);
        } else {
            battle_unit_store_coordinate_mount_animation_data(unit);
        }
    }
}
