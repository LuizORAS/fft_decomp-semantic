#include "fft/battle.h"
#include "psx/types.h"

/* Shift three words right by 12 bits, from 20.12 fixed point to whole units: the inverse of
 * battle_effect_shift_vector_left_12. battle_camera_execute_position_command calls it. */
void battle_effect_shift_vector_right_12(const s32* src, s32* dst) {
    dst[0] = src[0] >> 12;
    dst[1] = src[1] >> 12;
    dst[2] = src[2] >> 12;
}
