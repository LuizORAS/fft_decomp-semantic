#include "fft/battle_effect.h"

void battle_effect_add_vectors(const VECTOR* first, const VECTOR* second, VECTOR* out) {
    out->vx = first->vx + second->vx;
    out->vy = first->vy + second->vy;
    out->vz = first->vz + second->vz;
}
