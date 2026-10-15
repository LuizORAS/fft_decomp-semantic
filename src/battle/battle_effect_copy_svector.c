#include "psx/libgte.h"
#include "psx/types.h"

/* Copy three halfwords into an SVECTOR: the same keyframe moves read a keyframe's rotation this way. */
void battle_effect_copy_svector(s16* src, SVECTOR* dst) {
    dst->vx = src[0];
    dst->vy = src[1];
    dst->vz = src[2];
}
