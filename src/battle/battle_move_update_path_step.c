#include "fft/battle_effect.h"
#include "fft/battle_move.h"
#include "fft/battle_unit.h"
#include "fft/main.h"
#include "fft/sound.h"
#include "psx/types.h"

/*
 * Advance a walking unit by one frame: the elaborate sibling of
 * battle_move_update_knockback_step, with the full 0x3c-entry
 * step-phase table instead of the knockback subset.
 *
 * Phase 0x11/0x15/0x19/0x1d (jump crouch) starts the rise once the crouch animation has run out
 * (animation 0x1f, sound 0x27) and takes a rider off its mount; the teleport phases 0x3b (resets the
 * coordinates) and 0x3c (ends the walk once the animation is over) follow. With no step active the
 * path count selects the ending: 0xfe (a teleport) and 0xff (a failed one) dismount and start the
 * teleport step, and 0xff also drops the pending mount and restarts the AI action; 0 or an exhausted
 * path mounts the pending mount. Otherwise the next path byte starts a step: fly and float units
 * (BATTLE_EFFECTIVE_MOVEMENT_FLY, _FLOAT) have their own starters, BATTLE_MOVE_STEP_SOURCE_CLIMB
 * picks the climb speed, and battle_move_set_unit_path_flag marks the last step. */
void battle_move_update_path_step(battle_unit_t* unit) {
    u32 offset;
    u8 count;
    u8 flags;

    battle_move_get_current_and_destination_tiles(unit, &g_battle_move_current_tile, &g_battle_move_destination_tile);
    g_battle_move_step_value = unit->current_path_step;
    switch (unit->step_phase) {
    case BATTLE_MOVE_STEP_PHASE_JUMP_START_SOUTH:
    case BATTLE_MOVE_STEP_PHASE_JUMP_START_EAST:
    case BATTLE_MOVE_STEP_PHASE_JUMP_START_NORTH:
    case BATTLE_MOVE_STEP_PHASE_JUMP_START_WEST:
        if (unit->animation.wait == 0) {
            unit->step_phase++;
            battle_unit_store_animation_facing(BATTLE_UNIT_ANIMATION_FALL, unit->facing, unit);
            battle_sound_play_movement_sfx(unit, MAIN_SFX_JUMP);
            battle_unit_dismount_rider(unit);
        }
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
    case BATTLE_MOVE_STEP_PHASE_FLOAT_JUMP_SOUTH:
    case BATTLE_MOVE_STEP_PHASE_FLOAT_JUMP_EAST:
    case BATTLE_MOVE_STEP_PHASE_FLOAT_JUMP_NORTH:
    case BATTLE_MOVE_STEP_PHASE_FLOAT_JUMP_WEST:
        battle_move_update_float_jump_to_entry_edge(unit);
        break;
    case BATTLE_MOVE_STEP_PHASE_WALK_TO_CENTRE_SOUTH:
    case BATTLE_MOVE_STEP_PHASE_WALK_TO_CENTRE_EAST:
    case BATTLE_MOVE_STEP_PHASE_WALK_TO_CENTRE_NORTH:
    case BATTLE_MOVE_STEP_PHASE_WALK_TO_CENTRE_WEST:
    case BATTLE_MOVE_STEP_PHASE_CLIMB_TO_CENTRE_SOUTH:
    case BATTLE_MOVE_STEP_PHASE_CLIMB_TO_CENTRE_EAST:
    case BATTLE_MOVE_STEP_PHASE_CLIMB_TO_CENTRE_NORTH:
    case BATTLE_MOVE_STEP_PHASE_CLIMB_TO_CENTRE_WEST:
        battle_move_update_unit_step_to_destination_tile_center(unit);
        break;
    case BATTLE_MOVE_STEP_PHASE_FLOAT_TO_CENTRE_SOUTH:
    case BATTLE_MOVE_STEP_PHASE_FLOAT_TO_CENTRE_EAST:
    case BATTLE_MOVE_STEP_PHASE_FLOAT_TO_CENTRE_NORTH:
    case BATTLE_MOVE_STEP_PHASE_FLOAT_TO_CENTRE_WEST:
        battle_move_update_float_step_to_destination_tile_center(unit);
        break;
    case BATTLE_MOVE_STEP_PHASE_FLY_TO_CENTRE_SOUTH:
    case BATTLE_MOVE_STEP_PHASE_FLY_TO_CENTRE_EAST:
    case BATTLE_MOVE_STEP_PHASE_FLY_TO_CENTRE_NORTH:
    case BATTLE_MOVE_STEP_PHASE_FLY_TO_CENTRE_WEST:
        battle_move_update_fly_step_to_center(unit);
        break;
    case BATTLE_MOVE_STEP_PHASE_WALK_TO_EDGE_SOUTH:
    case BATTLE_MOVE_STEP_PHASE_WALK_TO_EDGE_EAST:
    case BATTLE_MOVE_STEP_PHASE_WALK_TO_EDGE_NORTH:
    case BATTLE_MOVE_STEP_PHASE_WALK_TO_EDGE_WEST:
    case BATTLE_MOVE_STEP_PHASE_CLIMB_TO_EDGE_SOUTH:
    case BATTLE_MOVE_STEP_PHASE_CLIMB_TO_EDGE_EAST:
    case BATTLE_MOVE_STEP_PHASE_CLIMB_TO_EDGE_NORTH:
    case BATTLE_MOVE_STEP_PHASE_CLIMB_TO_EDGE_WEST:
        battle_move_update_walking_step_at_tile_edge(unit);
        break;
    case BATTLE_MOVE_STEP_PHASE_FLOAT_TO_EDGE_SOUTH:
    case BATTLE_MOVE_STEP_PHASE_FLOAT_TO_EDGE_EAST:
    case BATTLE_MOVE_STEP_PHASE_FLOAT_TO_EDGE_NORTH:
    case BATTLE_MOVE_STEP_PHASE_FLOAT_TO_EDGE_WEST:
        battle_move_update_float_step_to_current_tile_exit_edge(unit);
        break;
    case BATTLE_MOVE_STEP_PHASE_FLY_TO_EDGE_SOUTH:
    case BATTLE_MOVE_STEP_PHASE_FLY_TO_EDGE_EAST:
    case BATTLE_MOVE_STEP_PHASE_FLY_TO_EDGE_NORTH:
    case BATTLE_MOVE_STEP_PHASE_FLY_TO_EDGE_WEST:
        battle_move_update_fly_step_to_exit_edge(unit);
        break;
    case BATTLE_MOVE_STEP_PHASE_TELEPORT_START:
        battle_move_update_teleport_start(unit);
        break;
    case BATTLE_MOVE_STEP_PHASE_TELEPORT_ANIMATION:
        battle_move_finalize_path_after_animation(unit);
        break;
    }
    if (unit->step_phase == BATTLE_MOVE_STEP_PHASE_NONE) {
        count = unit->path.count;
        if (count == BATTLE_MOVE_PATH_TELEPORT) {
            battle_unit_dismount_rider_and_update_display(unit);
            battle_move_start_teleport_step(unit);
        } else if (count == BATTLE_MOVE_PATH_TELEPORT_FAILED) {
            battle_unit_dismount_rider_and_update_display(unit);
            battle_move_start_teleport_step(unit);
            unit->path.pending_mount = 0;
            battle_ai_init_selected_action();
        } else if (count == 0) {
            battle_unit_mount_onto_pending_mount(unit);
        } else if (unit->movement_path_offset < count) {
            if (unit->movement_path_offset == count - 1) {
                battle_move_set_unit_path_flag(unit);
            }
            offset = unit->movement_path_offset;
            unit->movement_path_offset = offset + 1;
            unit->current_path_step = g_battle_move_step_value = unit->path.data.steps[offset];
            battle_move_set_unit_step_slope_scales(unit);
            flags = unit->path.flags;
            if (flags & BATTLE_EFFECTIVE_MOVEMENT_FLY) {
                battle_move_start_fly_step(unit, g_battle_move_current_tile, g_battle_move_destination_tile);
            } else if (flags & BATTLE_EFFECTIVE_MOVEMENT_FLOAT) {
                battle_move_start_float_step(unit, g_battle_move_current_tile, g_battle_move_destination_tile);
            } else {
                switch (g_battle_move_step_value.slow) {
                case 0:
                    unit->step_speed = 0x2000;
                    battle_move_start_unit_step(unit, g_battle_move_current_tile, g_battle_move_destination_tile);
                    break;
                case 1:
                    battle_move_start_unit_step_at_climb_speed(
                        unit, g_battle_move_current_tile, g_battle_move_destination_tile);
                    break;
                }
            }
        } else {
            battle_unit_mount_onto_pending_mount(unit);
        }
    }
}
