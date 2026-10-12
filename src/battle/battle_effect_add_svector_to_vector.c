#include "fft/battle_effect.h"

void battle_effect_add_svector_to_vector(const SVECTOR* first, const VECTOR* second, VECTOR* out) {
    out->vx = first->vx + second->vx;
    out->vy = first->vy + second->vy;
    out->vz = first->vz + second->vz;
}
