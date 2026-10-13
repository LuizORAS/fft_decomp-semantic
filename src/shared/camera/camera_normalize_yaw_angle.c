#include "fft/battle_camera.h"
#include "fft/script.h"

/* Reduce the yaw script variable to one turn, preserving the original whole-turn addition. */
void camera_normalize_yaw_angle(void) {
    g_script_variables[EVENT_SCRIPT_VAR_CAMERA_YAW]
        = (g_script_variables[EVENT_SCRIPT_VAR_CAMERA_YAW] + 0xa000) & 0x0fff;
}
