#include "fft/battle_effect.h"

void effect_add_vector_and_store_q12(const VECTOR* delta, VECTOR* value, VECTOR* out) {
    value->vx += delta->vx;
    out->vx = value->vx * ONE;
    value->vy += delta->vy;
    out->vy = value->vy * ONE;
    value->vz += delta->vz;
    out->vz = value->vz * ONE;
}
