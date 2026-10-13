#include "fft/battle_script.h"
#include "fft/script.h"
#include "psx/types.h"

/* RelativeCamera: rewrite the next Camera instruction's seven 16-bit operands
 * as the current camera values plus this instruction's deltas.
 *
 * src points at the seven camera script variables (x, z, y, tilt, yaw, roll,
 * zoom); the three positions are divided by 1024 first. A delta of
 * CAMERA_FUSION_HOLD_SENTINEL (10000) is written through unchanged, which
 * keeps that component. The Camera instruction is the one that follows, or the
 * one after a FocusSpeed that follows. */
void script_apply_relative_camera(event_camera_values_t* operands, s32* camera_values) {
    u8* parameters;
    s32* src;
    u8* dst;
    s32 i;
    s32 delta;
    s32 val;

    parameters = operands->position_le[0];
    src = camera_values;
    dst = ((event_camera_instruction_t*)(operands + 1))->parameters.values.position_le[0];
    if (((event_camera_instruction_t*)(operands + 1))->opcode == EVENT_OPCODE_FOCUS_SPEED) {
        dst = ((event_camera_instruction_t*)((event_focus_speed_instruction_t*)(operands + 1) + 1))
                  ->parameters.values.position_le[0];
    }
    for (i = 0; i < EVENT_CAMERA_COMPONENT_COUNT; i++) {
        delta = *src;
        if (i < EVENT_CAMERA_POSITION_COMPONENT_COUNT) {
            delta = delta / 1024;
        }
        if (script_read_s16(parameters) == CAMERA_FUSION_HOLD_SENTINEL) {
            val = CAMERA_FUSION_HOLD_SENTINEL;
        } else {
            val = delta + script_read_s16(parameters);
        }
        script_write_s16(dst, val);
        parameters += 2;
        dst += 2;
        src++;
    }
}
