#include "fft/battle_effect.h"

/* Shift three 32-bit values left by 12 bits, from whole units to 20.12 fixed point: the inverse of
 * battle_effect_shift_vector_right_12. battle_camera_update_effect_tracks calls it. */
void battle_effect_scale_vector_fixed12(const VECTOR* src, VECTOR* dst) {
    dst->vx = src->vx * ONE;
    dst->vy = src->vy * ONE;
    dst->vz = src->vz * ONE;
}
