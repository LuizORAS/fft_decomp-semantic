#include "fft/battle.h"
#include "psx/types.h"

/* Return 1 while the camera is moving: a pan step is left, or it is turning, tilting or zooming. The
 * event camera (battle_camera_wait_until_idle), the Triangle selection and the teleport animation wait for it
 * to stop, and the help menu does not open while it moves. */
s32 battle_camera_is_active(void) {
    if (g_battle_camera_step_vector.vx == 0 && g_battle_camera_step_vector.vy == 0
        && g_battle_camera_step_vector.vz == 0 && g_battle_camera_rotation_action == BATTLE_CAMERA_ROTATION_NONE
        && g_battle_camera_tilt_action == BATTLE_CAMERA_STEP_NONE
        && g_battle_camera_zoom_action == BATTLE_CAMERA_STEP_NONE) {
        return 0;
    }
    return 1;
}
