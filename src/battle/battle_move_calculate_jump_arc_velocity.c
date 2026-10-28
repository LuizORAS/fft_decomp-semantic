#include "fft/battle_move.h"
#include "psx/types.h"

/* Fill out with the velocity of a jump from `from` to `to` (screen coordinates) whose arc peaks 24
 * screen units (two height levels) above `to` under g_battle_move_jump_gravity: the upward launch
 * speed and the horizontal speeds that reach `to` in the flight time, which it returns in frames.
 *
 * The first call's argument is staged in `height` (the sum, then the product): one statement for
 * it changes the register allocation. The second call's 24 also goes through `height`: a literal 24
 * folds the product into a shift sequence, 8 bytes shorter than the target's li/mult. Loading to->vy
 * before from->vy gives the target's 16-bit load order. */
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
    out->vx = ((to->vx - from->vx) * ONE) / frames;
    out->vz = ((to->vz - from->vz) * ONE) / frames;
    return frames;
}
