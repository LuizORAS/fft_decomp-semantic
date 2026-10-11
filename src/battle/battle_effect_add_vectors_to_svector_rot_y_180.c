#include "fft/battle.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* Only the low 16 bits of each input component are read. */
/* result = left + right rotated 180 degrees about Y, i.e. (x - rx, y + ry, z - rz). */
void battle_effect_add_vectors_to_svector_rot_y_180(const VECTOR* left, const VECTOR* right, SVECTOR* result) {
    result->vx = left->vx - right->vx;
    result->vy = left->vy + right->vy;
    result->vz = left->vz - right->vz;
}
