#include "fft/battle_effect.h"

/* Convert integral vector components to 20.12 fixed point. */
void effect_scale_vector_fixed12(const VECTOR* src, VECTOR* dst) {
    dst->vx = src->vx * ONE;
    dst->vy = src->vy * ONE;
    dst->vz = src->vz * ONE;
}
