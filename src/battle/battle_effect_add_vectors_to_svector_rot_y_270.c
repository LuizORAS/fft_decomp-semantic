#include "psx/libgte.h"
#include "psx/types.h"

/* Same as battle_effect_add_vectors_to_svector with right turned 270 degrees about Y first, in
 * RotMatrixY's convention: (x - right z, y + right y, z + right x). */
void battle_effect_add_vectors_to_svector_rot_y_270(const VECTOR* left, const VECTOR* right, SVECTOR* result) {
    result->vx = left->vx - right->vz;
    result->vy = left->vy + right->vy;
    result->vz = left->vz + right->vx;
}
