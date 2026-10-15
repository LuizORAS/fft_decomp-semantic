#include "psx/types.h"

/* Add two three-word vectors into out. The effect camera's position and zoom commands and
 * battle_camera_update_effect_tracks use it. */
void battle_effect_add_vectors(const s32* first, const s32* second, s32* out) {
    out[0] = first[0] + second[0];
    out[1] = first[1] + second[1];
    out[2] = first[2] + second[2];
}
