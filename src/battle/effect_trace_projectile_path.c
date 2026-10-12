#include "fft/battle_effect.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/*
 * Step a projectile from `origin` along `delta` in 0x2000 increments of the
 * `distance` total, testing each position against the obstacle list. Stores
 * the last step's movement and the final position, sets g_effect_trajectory_step_count to a
 * quarter of the steps taken, and returns 1 when the path is clear or 0 when
 * an obstacle stopped it. The `step` variable keeps the target's register
 * divide; a literal 0x2000 folds to a shift.
 */
s32 effect_trace_projectile_path(
    VECTOR* delta, SVECTOR* origin, s32* distance, effect_obstacle_unit_list_t* obstacles) {
    VECTOR position;
    VECTOR previous;
    s32 step;
    s32 steps;
    s32 progress;
    s32 hit;
    s32 i;

    step = 0x2000;
    steps = *distance / step;
    progress = 0;
    position.vx = origin->vx;
    hit = 0;
    position.vy = origin->vy;
    i = 0;
    position.vz = origin->vz;
    for (; i < steps; i++) {
        previous = position;
        progress += step;
        effect_find_projectile_xyz_along_trajectory(delta, *distance, progress, &position);
        position.vx += origin->vx;
        position.vy += origin->vy;
        position.vz += origin->vz;
        hit = effect_test_position_for_obstacle(
            obstacles, &position, &g_effect_trajectory_hit_unit_id, &g_effect_trajectory_tile);
        if (hit != 0) {
            break;
        }
    }
    setVector(
        &g_effect_trajectory_step, position.vx - previous.vx, position.vy - previous.vy, position.vz - previous.vz);
    copyVector(&g_effect_trajectory_position, &position);
    if (hit != 0) {
        g_effect_trajectory_step_count = i / 4;
        return 0;
    }
    g_effect_trajectory_step_count = steps / 4;
    return 1;
}
