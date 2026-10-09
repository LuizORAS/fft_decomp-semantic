#include "fft/battle.h"
#include "psx/types.h"

s32 battle_camera_is_active(void) {
    if (g_battle_current_vector.vx == 0 && g_battle_current_vector.vy == 0 && g_battle_current_vector.vz == 0
        && g_battle_camera_rotation_action == BATTLE_CAMERA_ROTATION_NONE
        && g_battle_camera_tilt_action == BATTLE_CAMERA_STEP_NONE
        && g_battle_camera_zoom_action == BATTLE_CAMERA_STEP_NONE) {
        return 0;
    }
    return 1;
}
