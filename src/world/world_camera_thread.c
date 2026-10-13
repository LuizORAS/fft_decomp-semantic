/*
 * The event CAMERA instruction (0x19) thread. Instruction-for-instruction twin of the BATTLE routine at
 * 0x80146110; see that file for the interpolation modes and the
 * allocation-sensitive spellings.
 */
#include "fft/battle_camera.h"
#include "fft/script.h"
#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

#define ABS(x) (x < 0 ? -(x) : x)

/* Script-variable index for each camera component, 0x801531b8. */

void world_camera_thread(void) {
    s32 target[EVENT_CAMERA_COMPONENT_COUNT + 1];
    s32 start[EVENT_CAMERA_COMPONENT_COUNT];
    s32 i;
    s32 j;
    s32* variable;
    s32 group;
    s32 ease;
    s32 mode;
    s32 ramp;
    s32 weight;
    s32 duration;
    s32 half;
    s32 first;
    s32 accel;
    s32 elapsed;
    s32 delta;
    s32 decel;
    s32 base;
    s32 value;
    s32 angle;
    s16 yaw;
    s32 yaw_delta;
    const event_camera_parameters_t* parameters;
    const u8* script;

    mode = g_world_camera_speed_curve & EVENT_CAMERA_SPEED_CURVE_MODE_MASK;
    group = (g_world_camera_speed_curve & EVENT_CAMERA_SPEED_CURVE_GROUP_MASK) >> EVENT_CAMERA_SPEED_CURVE_GROUP_SHIFT;
    ease = (g_world_camera_speed_curve & EVENT_CAMERA_SPEED_CURVE_EASING_MASK) >> EVENT_CAMERA_SPEED_CURVE_EASING_SHIFT;
    thread_set_current_task_id(NATIVE_THREAD_TASK_CAMERA);
    parameters = world_thread_get_current_parameter_1();
    script = parameters->values.position_le[0];
    for (i = 0; i < EVENT_CAMERA_COMPONENT_COUNT + 1; i++) {
        target[i] = script_read_s16(script);
        script += 2;
    }

    if (g_world_camera_yaw_unwrap_request != 0) {
        g_world_camera_yaw_unwrap_request = 0;
        angle = script_get_variable(EVENT_SCRIPT_VAR_CAMERA_YAW);
        yaw_delta = ((angle << 16) >> 16) - target[EVENT_CAMERA_COMPONENT_YAW];
        yaw = angle;
        if (ABS(yaw_delta + ONE) < ABS(yaw_delta)) {
            yaw = angle + ONE;
        }
        script_set_variable(EVENT_SCRIPT_VAR_CAMERA_YAW, yaw);
    }

    for (i = 0; i < EVENT_CAMERA_COMPONENT_COUNT; i++) {
        start[i] = g_script_variables[g_camera_script_variable_indices[i]];
    }
    for (i = 0; i < EVENT_CAMERA_POSITION_COMPONENT_COUNT; i++) {
        start[i] &= ~0xFFF;
        start[i] /= 4;
    }

    duration = target[EVENT_CAMERA_COMPONENT_COUNT];
    for (i = 0; i < target[EVENT_CAMERA_COMPONENT_COUNT]; i++) {
        weight = 16 - ease;
        ramp = (duration - i) * i;
        for (j = 0; j < EVENT_CAMERA_COMPONENT_COUNT; j++) {
            variable = &g_script_variables[g_camera_script_variable_indices[j]];
            if (target[j] == CAMERA_FUSION_HOLD_SENTINEL) {
                continue;
            }
            if (g_world_camera_position_locked != 0 && j < EVENT_CAMERA_POSITION_COMPONENT_COUNT) {
                continue;
            }
            if (j < EVENT_CAMERA_POSITION_COMPONENT_COUNT) {
                delta = (target[j] << 8) - start[j];
                base = start[j];
                if (ABS(delta) < 0x600) {
                    continue;
                }
            } else {
                delta = (target[j] - start[j]) << 6;
                base = start[j] << 6;
                if (ABS(delta) < 0x60) {
                    continue;
                }
            }
            if (mode == 0 || (group == 0 && j >= EVENT_CAMERA_COMPONENT_TILT)
                || (group == 1 && j < EVENT_CAMERA_POSITION_COMPONENT_COUNT)) {
                value = world_mul_div_64(delta, i, duration) + base;
                if (j < EVENT_CAMERA_POSITION_COMPONENT_COUNT) {
                    value *= 4;
                }
            } else if (mode == 1) {
                decel = delta * weight;
                accel = delta * ease * 2;
                first = world_mul_div_64(accel + decel + decel, i, duration * 32);
                value = world_mul_div_64(accel, ramp, duration * duration * 32) + first + base;
                if (j < EVENT_CAMERA_POSITION_COMPONENT_COUNT) {
                    value *= 4;
                }
            } else {
                delta *= 2;
                half = i * 2;
                if (half < duration) {
                    decel = delta * weight;
                    value = world_mul_div_64(
                        world_mul_div_64(delta * ease * 2, half, duration) + decel + decel, half, duration * 32);
                    if (j < EVENT_CAMERA_POSITION_COMPONENT_COUNT) {
                        value += base * 4;
                    } else {
                        value = value / 4 + base;
                    }
                } else {
                    decel = delta * weight;
                    accel = delta * ease * 2;
                    elapsed = half - duration;
                    value = world_mul_div_64(
                        accel + world_mul_div_64(accel, duration - elapsed, duration) + decel + decel, elapsed,
                        duration * 32);
                    if (j < EVENT_CAMERA_POSITION_COMPONENT_COUNT) {
                        value += delta;
                        value += base * 4;
                    } else {
                        value = (value + delta) / 4 + base;
                    }
                }
            }
            if (j >= EVENT_CAMERA_COMPONENT_TILT) {
                if (value > 0) {
                    if ((value & 0xFF) > 0x20) {
                        value += 0x40;
                    }
                } else if ((value & 0xFF) < 0x20) {
                    value -= 0x40;
                }
                value /= 64;
            }
            *variable = value;
        }
        camera_clear_zoom_variable_low_bit();
        thread_yield();
    }

    if (g_world_camera_position_locked == 0) {
        for (i = 0; i < EVENT_CAMERA_POSITION_COMPONENT_COUNT; i++) {
            if (target[i] != CAMERA_FUSION_HOLD_SENTINEL) {
                g_script_variables[g_camera_script_variable_indices[i]] = target[i] << 10;
            }
        }
    }
    for (i = EVENT_CAMERA_COMPONENT_TILT; i < EVENT_CAMERA_COMPONENT_COUNT; i++) {
        if (target[i] != CAMERA_FUSION_HOLD_SENTINEL) {
            g_script_variables[g_camera_script_variable_indices[i]] = target[i];
        }
    }
    camera_clear_zoom_variable_low_bit();
    thread_yield();
    thread_exit_current();
}
