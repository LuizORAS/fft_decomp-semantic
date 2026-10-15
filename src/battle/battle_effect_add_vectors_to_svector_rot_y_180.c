#include "psx/libgte.h"
#include "psx/types.h"

/* Same as battle_effect_add_vectors_to_svector with right turned 180 degrees about Y first: (x - right x,
 * y + right y, z - right z). */
void battle_effect_add_vectors_to_svector_rot_y_180(const VECTOR* left, const VECTOR* right, SVECTOR* result) {
    result->vx = left->vx - right->vx;
    result->vy = left->vy + right->vy;
    result->vz = left->vz - right->vz;
}
