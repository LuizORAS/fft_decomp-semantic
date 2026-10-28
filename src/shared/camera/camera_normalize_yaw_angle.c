#include "fft/battle_camera.h"
#include "fft/script.h"

/* Wraps the yaw script variable into 0..0xfff. The 0xa000 added first is a
 * multiple of a full turn and does not change the result. */
void camera_normalize_yaw_angle(void) {
    g_script_variables[EVENT_SCRIPT_VAR_CAMERA_YAW]
        = (g_script_variables[EVENT_SCRIPT_VAR_CAMERA_YAW] + 0xa000) & 0x0fff;
}
