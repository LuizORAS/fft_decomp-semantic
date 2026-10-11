#include "fft/battle.h"
#include "psx/types.h"

void battle_effect_add_vector_and_store_q12(const VECTOR* delta, VECTOR* value, VECTOR* out) {
    value->vx += delta->vx;
    out->vx = value->vx << 12;
    value->vy += delta->vy;
    out->vy = value->vy << 12;
    value->vz += delta->vz;
    out->vz = value->vz << 12;
}
