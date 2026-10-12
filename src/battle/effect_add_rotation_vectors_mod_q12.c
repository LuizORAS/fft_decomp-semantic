#include "fft/battle_effect.h"

enum {
    BATTLE_ROTATION_Q12_MASK = 0x0FFF,
};

/* Add two effect rotations and wrap each axis to one Q12 revolution. */
void effect_add_rotation_vectors_mod_q12(const SVECTOR* first, const SVECTOR* second, SVECTOR* out) {
    out->vx = (first->vx + second->vx) & BATTLE_ROTATION_Q12_MASK;
    out->vy = (first->vy + second->vy) & BATTLE_ROTATION_Q12_MASK;
    out->vz = (first->vz + second->vz) & BATTLE_ROTATION_Q12_MASK;
}
