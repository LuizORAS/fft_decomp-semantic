#include "fft/world.h"
#include "psx/types.h"

/*
 * Writes the parameters of the camera instruction that follows a relative
 * camera instruction: each of the seven halfwords at `source` plus its
 * delta, stored 15 bytes on (18 when a focus-speed instruction comes
 * first). The first three deltas (the position) are divided by 1024; a
 * halfword holding the "hold" sentinel is passed through unchanged.
 */
void world_script_apply_relative_camera(event_camera_values_t* parameters, const s32* camera_values) {
    u8* source;
    const s32* deltas;
    u8* destination;
    s32 index;
    s32 delta;
    s32 value;

    source = parameters->position_le[0];
    deltas = camera_values;
    destination = ((event_camera_instruction_t*)(parameters + 1))->parameters.values.position_le[0];
    if (((event_camera_instruction_t*)(parameters + 1))->opcode == EVENT_OPCODE_FOCUS_SPEED) {
        destination = ((event_camera_instruction_t*)((event_focus_speed_instruction_t*)(parameters + 1) + 1))
                          ->parameters.values.position_le[0];
    }

    for (index = 0; index < EVENT_CAMERA_COMPONENT_COUNT; index++) {
        delta = *deltas;
        if (index < EVENT_CAMERA_POSITION_COMPONENT_COUNT) {
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
