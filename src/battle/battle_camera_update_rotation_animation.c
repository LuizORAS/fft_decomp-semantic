#include "fft/battle_camera.h"

/* Advances the camera-angle move by one frame while its countdown runs. Only the
 * uncalled battle_camera_set_rotation_delta_* routines start the countdown, so
 * nothing moves in retail. */
void battle_camera_update_rotation_animation(void) {
    if (g_battle_camera_rotation_countdown != 0) {
        g_battle_camera_rotation_countdown--;
        g_battle_camera_angles.vx += g_battle_camera_rotation_delta.vx;
        g_battle_camera_angles.vy += g_battle_camera_rotation_delta.vy;
        g_battle_camera_angles.vz += g_battle_camera_rotation_delta.vz;
    }
}
