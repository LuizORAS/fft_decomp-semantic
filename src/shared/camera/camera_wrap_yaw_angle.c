#include "fft/battle_camera.h"
#include "fft/script.h"
#include "psx/types.h"

/* Raise negative yaw by whole turns; retain positive values of ONE or greater. */
s32 camera_wrap_yaw_angle(void) {
    s32 yaw;

    yaw = g_script_variables[EVENT_SCRIPT_VAR_CAMERA_YAW];
    while (yaw < 0) {
        yaw += ONE;
    }
    return yaw;
}
