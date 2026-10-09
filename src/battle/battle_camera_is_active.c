#include "fft/battle.h"
#include "psx/types.h"

s32 battle_camera_is_active(void) {
    if (g_battle_current_vector.vx == 0 && g_battle_current_vector.vy == 0 && g_battle_current_vector.vz == 0
        && g_battle_camera_rotation_action == 0 && g_battle_camera_tilt_action == 0
        && g_battle_camera_zoom_action == 0) {
        return 0;
    }
    return 1;
}
