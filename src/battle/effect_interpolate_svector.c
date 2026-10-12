#include "fft/battle_effect.h"
#include "psx/types.h"

void effect_interpolate_svector(const SVECTOR* from, const SVECTOR* to, s32 t, SVECTOR* out) {
    effect_interpolate_s16(from->vx, to->vx, t, &out->vx);
    effect_interpolate_s16(from->vy, to->vy, t, &out->vy);
    effect_interpolate_s16(from->vz, to->vz, t, &out->vz);
}
