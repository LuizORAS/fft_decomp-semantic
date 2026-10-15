#include "fft/battle.h"
#include "psx/types.h"

/* Shift three words left by 12 bits, from whole units to 20.12 fixed point: the inverse of
 * battle_effect_shift_vector_right_12. battle_camera_update_effect_tracks calls it. */
void battle_effect_shift_vector_left_12(const VECTOR* src, VECTOR* dst) {
    dst->vx = src->vx << 12;
    dst->vy = src->vy << 12;
    dst->vz = src->vz << 12;
}
