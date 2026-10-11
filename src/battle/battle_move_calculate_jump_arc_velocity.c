#include "fft/battle.h"
#include "psx/types.h"

/* Fill out with the velocity of a jump from `from` to `to` (screen coordinates) whose arc peaks 24
 * screen units (two height levels) above `to` under g_battle_move_jump_gravity: the upward launch
 * speed and the horizontal speeds that reach `to` in the flight time, which it returns in frames.
 *
 * The first call's argument is staged in one `height` variable (sum, then product), which
 * local_alloc ties to $a0 through the argument copy. The height difference gets its own `height`,
 * and the empty asm input keeps it live past the sum: while `height` dies there, the sum ties to
 * height's $v0 instead of the target's $a0. The `apex = 24` local is required: with a literal 24
 * the second call's argument folds to a shift sequence instead of the target's li/mult. The
 * zt-before-from_z local loads give the target's halfword load order. */
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
