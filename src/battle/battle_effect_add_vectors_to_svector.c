#include "fft/battle_effect.h"
#include "psx/libgte.h"

/* Only the low 16 bits of each input component are read. */
/* result = left + right */
void battle_effect_add_vectors_to_svector(const VECTOR* left, const VECTOR* right, SVECTOR* result) {
    result->vx = left->vx + right->vx;
    result->vy = left->vy + right->vy;
    result->vz = left->vz + right->vz;
}
