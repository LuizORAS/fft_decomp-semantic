#include "fft/battle_effect.h"

/* Shift three 32-bit values right by 12 bits, from 20.12 fixed point to whole units: the inverse of
 * battle_effect_scale_vector_q12. battle_camera_execute_position_command calls it. */
void battle_effect_shift_vector_right_12(const VECTOR* src, VECTOR* dst) {
    dst->vx = src->vx >> 12;
    dst->vy = src->vy >> 12;
    dst->vz = src->vz >> 12;
}
