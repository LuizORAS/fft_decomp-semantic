#include "fft/battle_camera.h"
#include "psx/types.h"

/* Start a right turn to target_angle from the slowest speed. */
void battle_camera_call_rotate_right(s32 target_angle) {
    g_battle_camera_rotation_action = BATTLE_CAMERA_ROTATION_RIGHT;
    g_battle_camera_rotation_increment = 1;
    battle_camera_rotate_right(target_angle);
}
