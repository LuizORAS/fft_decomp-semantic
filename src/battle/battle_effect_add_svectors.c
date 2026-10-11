#include "fft/battle.h"
#include "psx/libgte.h"
#include "psx/types.h"

void battle_effect_add_svectors(const SVECTOR* first, const SVECTOR* second, SVECTOR* out) {
    out->vx = first->vx + second->vx;
    out->vy = first->vy + second->vy;
    out->vz = first->vz + second->vz;
}
