#include "fft/battle_effect.h"
#include "psx/libgte.h"

/* Add two VECTORs into an SVECTOR: only the low 16 bits of each component reaches the result. The 32
 * ring mesh renderers of the effect overlays place their points with it and its three rotated forms. */
void battle_effect_add_vectors_to_svector(const VECTOR* left, const VECTOR* right, SVECTOR* result) {
    result->vx = left->vx + right->vx;
    result->vy = left->vy + right->vy;
    result->vz = left->vz + right->vz;
}
