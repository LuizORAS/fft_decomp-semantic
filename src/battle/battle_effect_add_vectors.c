#include "fft/battle_effect.h"

/* Add two vectors of three 32-bit values into out. The effect camera's position and zoom commands and
 * battle_camera_update_effect_tracks use it. */
void battle_effect_add_vectors(const VECTOR* first, const VECTOR* second, VECTOR* out) {
    out->vx = first->vx + second->vx;
    out->vy = first->vy + second->vy;
    out->vz = first->vz + second->vz;
}
