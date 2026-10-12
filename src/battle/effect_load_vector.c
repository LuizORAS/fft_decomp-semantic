#include "fft/battle_effect.h"
#include "psx/libgte.h"

void effect_load_vector(vec3s16_t* src, VECTOR* dst) {
    dst->vx = src->x;
    dst->vy = src->y;
    dst->vz = src->z;
}
