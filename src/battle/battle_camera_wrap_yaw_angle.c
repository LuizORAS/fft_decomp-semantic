#include "fft/battle_camera.h"
#include "fft/battle_event.h"
#include "psx/types.h"

/* The yaw script variable raised by whole turns until it is not negative. A yaw
 * of 0x1000 or more is returned as it is. */
s32 battle_camera_wrap_yaw_angle(void) {
    s32 yaw;

    yaw = g_battle_script_variables[EVENT_SCRIPT_VAR_CAMERA_YAW];
    while (yaw < 0) {
        yaw += ONE;
    }
    return yaw;
}
