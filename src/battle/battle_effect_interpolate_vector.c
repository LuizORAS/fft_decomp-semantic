#include "fft/battle_effect.h"
#include "psx/types.h"

/* Same as battle_effect_interpolate_svector on three 32-bit values, through battle_effect_interpolate_s32. */
void battle_effect_interpolate_vector(const VECTOR* from, const VECTOR* to, s32 t, VECTOR* out) {
    battle_effect_interpolate_s32(from->vx, to->vx, t, &out->vx);
    battle_effect_interpolate_s32(from->vy, to->vy, t, &out->vy);
    battle_effect_interpolate_s32(from->vz, to->vz, t, &out->vz);
}
