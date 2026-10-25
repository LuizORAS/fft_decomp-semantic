#include "fft/battle_gfx.h"
#include "fft/battle_map.h"
#include "fft/battle_move.h"
#include "fft/battle_unit.h"
#include "fft/map.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Distortion 6 (BATTLE_DISTORTION_ANIMATION_JUMP_TO_TARGET): jump along an arc toward the first target's
 * sprite center (half its sprite height above it), or the action target tile when the unit has no
 * targets, and end when the flight frames run out, without landing. Phase 0 asks
 * battle_move_calculate_jump_arc_velocity for the velocity and frame count; phase 1 applies it,
 * accelerating the height by g_battle_move_jump_gravity. A unit with no battle record ends at once. */
void battle_move_animate_jump_arc_to_target(battle_unit_t* unit) {
    SVECTOR target;
    /* The retail frame reserves eight bytes beyond the target vector. */
    u8 unused_target_tail[8];
    VECTOR velocity;
    battle_unit_t* other;
    s32 timer;

    timer = unit->distortion_timer;
    velocity = unit->velocity;
    switch (unit->distortion_phase) {
    case 0:
        if (unit->stats != 0) {
            if (unit->strike.target_count != 0) {
                other = battle_unit_get_by_battle_id(unit->strike.target_list[0]);
                setVector(&target, other->world_coords.vx,
                    other->world_coords.vy
                        - battle_gfx_get_unit_spritesheet_height_by_runtime_id(other->runtime_id) / 2,
                    other->world_coords.vz);
            } else {
                target.vx = unit->stats->action_command.target_position.vx * BATTLE_MAP_TILE_SIZE
                    + (BATTLE_MAP_TILE_SIZE / 2);
                target.vz = unit->stats->action_command.target_position.vz * BATTLE_MAP_TILE_SIZE
                    + (BATTLE_MAP_TILE_SIZE / 2);
                target.vy = battle_map_calculate_slope_height(&target, unit->stats->action_command.target_position.vy);
            }
            timer = battle_move_calculate_jump_arc_velocity(&unit->world_coords, &target, &velocity);
            unit->distortion_phase++;
        } else {
            unit->distortion_animation_id = BATTLE_DISTORTION_ANIMATION_NONE;
            break;
        }
    case 1:
        if (timer > 0) {
            unit->real.vx += velocity.vx;
            unit->real.vy += velocity.vy;
            unit->real.vz += velocity.vz;
            velocity.vy += g_battle_move_jump_gravity;
        } else {
            unit->distortion_animation_id = BATTLE_DISTORTION_ANIMATION_NONE;
        }
        timer--;
        break;
    }
    unit->velocity = velocity;
    battle_unit_set_world_coords_from_real_coords(unit);
    unit->distortion_timer = timer;
}
