#include "fft/battle_effect.h"
#include "psx/libgte.h"

void battle_effect_load_svector(vec3s16_t* src, SVECTOR* dst) {
    dst->vx = src->x;
    dst->vy = src->y;
    dst->vz = src->z;
}
