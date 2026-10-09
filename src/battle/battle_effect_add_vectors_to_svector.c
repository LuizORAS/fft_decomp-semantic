#include "psx/libgte.h"
#include "psx/types.h"

/* Only the low halfword of each input component is read. */
/* result = left + right */
void battle_effect_add_vectors_to_svector(const VECTOR* left, const VECTOR* right, SVECTOR* result) {
    result->vx = left->vx + right->vx;
    result->vy = left->vy + right->vy;
    result->vz = left->vz + right->vz;
}
