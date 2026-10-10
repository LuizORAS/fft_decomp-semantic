#include "fft/world.h"
#include "psx/types.h"

/*
 * Writes the parameters of the camera instruction that follows a relative
 * camera instruction: each of the seven halfwords at `source` plus its
 * delta, stored 15 bytes on (18 when a focus-speed instruction comes
 * first). The first three deltas (the position) are divided by 1024; a
 * halfword holding the "hold" sentinel is passed through unchanged.
 */
void world_script_apply_relative_camera(u8* source, const s32* deltas) {
    u8* destination;
    s32 index;
    s32 delta;
    s32 value;

    destination = source + 15;
    if (source[14] == EVENT_OPCODE_FOCUS_SPEED) {
        destination = source + 18;
    }

    for (index = 0; index < 7; index++) {
        delta = *deltas;
        if (index < 3) {
            delta = delta / 1024;
        }
        if (world_script_load_halfword(source) == BATTLE_CAMERA_FUSION_HOLD_SENTINEL) {
            value = BATTLE_CAMERA_FUSION_HOLD_SENTINEL;
        } else {
            value = delta + world_script_load_halfword(source);
        }
        world_script_store_halfword(destination, value);
        source += 2;
        destination += 2;
        deltas++;
    }
}
