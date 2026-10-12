#include "fft/battle_camera.h"
#include "fft/battle_event.h"

/* Wraps the yaw script variable into 0..0xfff. The 0xa000 added first is a
 * multiple of a full turn and does not change the result. */
void battle_camera_normalize_yaw_angle(void) {
    g_battle_script_variables[EVENT_SCRIPT_VAR_CAMERA_YAW]
        = (g_battle_script_variables[EVENT_SCRIPT_VAR_CAMERA_YAW] + 0xa000) & 0x0fff;
}
