#include "fft/battle.h"
#include "psx/types.h"

void battle_effect_interpolate_svector(const SVECTOR* from, const SVECTOR* to, s32 t, SVECTOR* out) {
    battle_effect_interpolate_s16(from->vx, to->vx, t, &out->vx);
    battle_effect_interpolate_s16(from->vy, to->vy, t, &out->vy);
    battle_effect_interpolate_s16(from->vz, to->vz, t, &out->vz);
}
