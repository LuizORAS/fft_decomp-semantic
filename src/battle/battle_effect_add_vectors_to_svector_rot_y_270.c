#include "fft/battle.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* Only the low 16 bits of each input component are read. */
/* result = left + right rotated 270 degrees about Y (RotMatrixY convention:
 * x' = x cos + z sin, z' = -x sin + z cos), i.e. (x - rz, y + ry, z + rx). */
void battle_effect_add_vectors_to_svector_rot_y_270(const VECTOR* left, const VECTOR* right, SVECTOR* result) {
    result->vx = left->vx - right->vz;
    result->vy = left->vy + right->vy;
    result->vz = left->vz + right->vx;
}
