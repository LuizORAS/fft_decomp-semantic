#include "fft/battle_effect.h"

/* Add three 16-bit values to three 32-bit values into out (battle_effect_spawn_emitter_particles). */
void battle_effect_add_svector_to_vector(const SVECTOR* first, const VECTOR* second, VECTOR* out) {
    out->vx = first->vx + second->vx;
    out->vy = first->vy + second->vy;
    out->vz = first->vz + second->vz;
}
