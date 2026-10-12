#include "fft/battle_effect.h"
#include "psx/libgte.h"
#include "psx/types.h"

/*
 * Evaluate the arcing-weapon trajectory term a * b - (1 + a^2) * b^2 /
 * (2 * g_effect_gravity_modifier) in 20.12 fixed point.
 *
 * This is the height of a ballistic arc after a horizontal distance b
 * (`progress`), where a (`height`) is the tangent of the launch pitch in Q12:
 * the callers take the pitch itself as ratan2(height, ONE).
 *
 * Both operands are pre-shifted by range so the squares stay within 32 bits.
 * g_effect_gravity_modifier is the trajectory gravity modifier
 * (0xa8000).
 */
s32 effect_calculate_arc_trajectory_term(s32 height, s32 progress) {
    s32 divisor;
    s32 height_sq;
    s32 progress_sq;
    s32 linear;

    divisor = (g_effect_gravity_modifier * 2) >> 12;
    if (height >= -0x7fff && height <= 0x7fff) {
        height_sq = (height * height) >> 12;
        if (progress >= -0xfffff && progress <= 0xfffff) {
            progress_sq = (progress >> 5) * (progress >> 5);
            progress_sq >>= 2;
            linear = (height * (progress >> 5)) >> 7;
        } else {
            progress_sq = (progress >> 7) * (progress >> 7);
            progress_sq <<= 2;
            linear = (height * (progress >> 7)) >> 5;
        }
    } else {
        height >>= 4;
        height_sq = (height * height) >> 4;
        if (progress >= -0xfffff && progress <= 0xfffff) {
            progress_sq = (progress >> 5) * (progress >> 5);
            progress_sq >>= 2;
            linear = (height * (progress >> 5)) >> 3;
        } else {
            progress_sq = (progress >> 7) * (progress >> 7);
            progress_sq <<= 2;
            linear = (height * (progress >> 7)) >> 1;
        }
    }
    return linear - (((height_sq + ONE) >> 6) * (progress_sq >> 6)) / divisor;
}
