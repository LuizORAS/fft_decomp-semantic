#include "fft/battle_effect.h"

/* Add delta to value in place, and store each sum in out shifted left by 12, into 20.12 fixed point
 * (battle_camera_update_effect_tracks). */
void battle_effect_add_vector_and_store_q12(const VECTOR* delta, VECTOR* value, VECTOR* out) {
    value->vx += delta->vx;
    out->vx = value->vx * ONE;
    value->vy += delta->vy;
    out->vy = value->vy * ONE;
    value->vz += delta->vz;
    out->vz = value->vz * ONE;
}
