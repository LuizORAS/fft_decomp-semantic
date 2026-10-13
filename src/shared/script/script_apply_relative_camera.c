#include "fft/script.h"

/* Rewrite the following Camera operands, skipping an intervening FocusSpeed instruction. */
void script_apply_relative_camera(event_camera_values_t* parameters, const s32* camera_values) {
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
        if (script_read_s16(source) == CAMERA_FUSION_HOLD_SENTINEL) {
            value = CAMERA_FUSION_HOLD_SENTINEL;
        } else {
            value = delta + script_read_s16(source);
        }
        script_write_s16(destination, value);
        source += 2;
        destination += 2;
        deltas++;
    }
}
