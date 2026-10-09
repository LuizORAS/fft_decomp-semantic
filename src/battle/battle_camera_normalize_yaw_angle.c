#include "fft/battle.h"
#include "psx/types.h"

void battle_camera_normalize_yaw_angle(void) {
    g_battle_script_variables[EVENT_SCRIPT_VAR_CAMERA_YAW]
        = (g_battle_script_variables[EVENT_SCRIPT_VAR_CAMERA_YAW] + 0xa000) & 0x0fff;
}
