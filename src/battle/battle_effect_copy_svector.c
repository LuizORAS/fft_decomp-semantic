#include "fft/battle_effect.h"
#include "psx/libgte.h"

/* Copy three 16-bit values into an SVECTOR: the same keyframe moves read a keyframe's rotation this way. */
void battle_effect_copy_svector(const vec3s16_t* src, SVECTOR* dst) {
    dst->vx = src->x;
    dst->vy = src->y;
    dst->vz = src->z;
}
