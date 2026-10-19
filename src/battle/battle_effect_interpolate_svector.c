#include "fft/battle_effect.h"
#include "psx/types.h"

/* Interpolate three 16-bit values by t, a fraction of 0x2000, with battle_effect_interpolate_s16. */
void battle_effect_interpolate_svector(const SVECTOR* from, const SVECTOR* to, s32 t, SVECTOR* out) {
    battle_effect_interpolate_s16(from->vx, to->vx, t, &out->vx);
    battle_effect_interpolate_s16(from->vy, to->vy, t, &out->vy);
    battle_effect_interpolate_s16(from->vz, to->vz, t, &out->vz);
}
