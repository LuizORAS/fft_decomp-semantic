#include "fft/script.h"

/* RelativeCamera: rewrite the next Camera instruction's seven operand 16-bit values
 * as the current camera values plus this instruction's deltas; BATTLE and WORLD compile this text.
 *
 * deltas points at the seven camera script variables (x, z, y, tilt, yaw, roll,
 * zoom); the three positions are divided by 1024 first. A delta of
 * BATTLE_CAMERA_FUSION_HOLD_SENTINEL (10000) is written through unchanged, which
 * keeps that component. The Camera instruction is the one that follows, 15 bytes on, or the
 * one after a FocusSpeed that follows (18 bytes on). */
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
        if (script_load_halfword(source) == BATTLE_CAMERA_FUSION_HOLD_SENTINEL) {
            value = BATTLE_CAMERA_FUSION_HOLD_SENTINEL;
        } else {
            value = delta + script_load_halfword(source);
        }
        script_store_halfword(destination, value);
        source += 2;
        destination += 2;
        deltas++;
    }
}
