#include "fft/battle.h"
#include "psx/types.h"

/* Advance a knocked-back target by one frame (battle_state_handle_action_execute_state runs it for
 * every target of the action). A cut-down battle_move_update_path_step: while a step is in progress
 * its speed slows by a quarter per frame down to ONE (battle_move_init_knockback starts it at 8.0)
 * and the walk, jump and landing phase handlers run; then the next path step starts, or, once the
 * path is done, battle_unit_store_coordinate_mount_animation_data runs. */
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
            battle_move_update_landing_phase(unit);
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
    if (unit->movement_path_count != 0) {
        offset = unit->movement_path_offset;
        if (offset < unit->movement_path_count) {
            unit->movement_path_offset = offset + 1;
            unit->current_path_step = g_battle_move_step_value = unit->movement_path[offset];
            battle_move_set_unit_step_slope_scales(unit);
            unit->step_speed = 0x2000;
            battle_move_start_unit_step(unit, g_battle_move_current_tile, g_battle_move_destination_tile);
        } else {
            battle_unit_store_coordinate_mount_animation_data(unit);
        }
    }
}
