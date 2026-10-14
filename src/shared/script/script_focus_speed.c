#include "fft/script.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* FocusSpeed event instruction: derives the camera move duration from the
 * distance between the current and requested position/rotation and the speed.
 *
 * The original leaves rotation_sum uninitialized (s6 is never set). */
void script_focus_speed(u8* parameters, s32* position, s32* rotation) {
    s32 position_delta[4]; /* four slots: the target spaces the arrays 0x10 apart */
    s32 rotation_delta[4];
    s32 speed;
    s32 i;
    s32 value;
    s32 position_sum;
    s32 rotation_sum;

    /* A separate typed Camera cursor reorders the original argument saves. */
    speed = script_read_s16(((event_focus_speed_parameters_t*)parameters)->speed_le);
    parameters += sizeof(event_focus_speed_parameters_t);
    parameters = ((event_camera_instruction_t*)parameters)->parameters.values.position_le[0];
    position_sum = 0;
    for (i = 0; i < 3; i++) {
        value = script_read_s16(parameters + i * 2);
        if (value == CAMERA_FUSION_HOLD_SENTINEL) {
            position_delta[i] = 0;
        } else {
            position_delta[i] = value - position[i] / 1024;
        }
        value = script_read_s16(parameters + i * 2 + sizeof(((event_camera_values_t*)0)->position_le));
        if (value == CAMERA_FUSION_HOLD_SENTINEL) {
            rotation_delta[i] = 0;
        } else {
            rotation_delta[i] = value - rotation[i];
        }
        position_sum += position_delta[i] * position_delta[i];
        rotation_sum += rotation_delta[i] * rotation_delta[i];
    }
    if (position_sum * 2 < rotation_sum) {
        position_sum = rotation_sum / 2;
    }
    position_sum = SquareRoot0(position_sum) / speed;
    if (position_sum == 0) {
        position_sum = 1;
    }
    script_write_s16(((event_camera_parameters_t*)parameters)->duration_le, position_sum);
}
