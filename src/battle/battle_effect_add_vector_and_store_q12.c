#include "psx/types.h"

/* Add delta to value in place, and store each sum in out shifted left by 12, into 20.12 fixed point
 * (battle_camera_update_effect_tracks). */
void battle_effect_add_vector_and_store_q12(const s32* delta, s32* value, s32* out) {
    value[0] += delta[0];
    out[0] = value[0] << 12;
    value[1] += delta[1];
    out[1] = value[1] << 12;
    value[2] += delta[2];
    out[2] = value[2] << 12;
}
