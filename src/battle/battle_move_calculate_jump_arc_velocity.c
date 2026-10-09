#include "fft/battle.h"
#include "psx/types.h"

/* Velocity and duration of a jump arc from `from` to `to`.
 *
 * The arc has two legs under g_battle_move_jump_gravity, each taking
 * sqrt(2 * gravity * height) / gravity frames: one over the height difference
 * plus 24, one over 24. `out->vy` is the negated speed of the first leg; x and
 * y (stored in vz) cover their distances evenly over the returned frame count. */
s32 battle_move_calculate_jump_arc_velocity(const SVECTOR* from, const SVECTOR* to, VECTOR* out) {
    s32 to_z;
    s32 from_z;
    s32 height;
    s32 speed;
    s32 frames;

    to_z = to->vy;
    from_z = from->vy;
    height = from_z - to_z + 24;
    height = height * g_battle_move_jump_gravity;
    speed = SquareRoot12(height * 2);
    frames = speed / g_battle_move_jump_gravity;
    out->vy = -speed;
    height = 24;
    height = g_battle_move_jump_gravity * height;
    speed = SquareRoot12(height * 2);
    frames += speed / g_battle_move_jump_gravity;
    out->vx = ((to->vx - from->vx) << 12) / frames;
    out->vz = ((to->vz - from->vz) << 12) / frames;
    return frames;
}
