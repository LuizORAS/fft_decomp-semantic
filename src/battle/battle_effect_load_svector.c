#include "fft/battle.h"
#include "psx/libgte.h"
#include "psx/types.h"

void battle_effect_load_svector(vec3s16_t* src, SVECTOR* dst) {
    dst->vx = src->x;
    dst->vy = src->y;
    dst->vz = src->z;
}
