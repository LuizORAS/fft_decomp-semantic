#include "fft/world.h"

/* Reduce the camera-yaw script variable to 0..0xfff (one turn is 0x1000). */
void world_camera_init_yaw_angle(void) {
    g_world_script_variables[EVENT_SCRIPT_VAR_CAMERA_YAW]
        = (g_world_script_variables[EVENT_SCRIPT_VAR_CAMERA_YAW] + 0xa000) & 0x0fff;
}
