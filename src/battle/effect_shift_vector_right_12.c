#include "fft/battle_effect.h"

void effect_shift_vector_right_12(const VECTOR* src, VECTOR* dst) {
    dst->vx = src->vx >> 12;
    dst->vy = src->vy >> 12;
    dst->vz = src->vz >> 12;
}
