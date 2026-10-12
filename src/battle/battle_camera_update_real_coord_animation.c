#include "fft/battle_camera.h"

/* Advances the camera-position move by one frame while its countdown runs. Only
 * the uncalled battle_camera_set_real_coord_delta_* routines start the countdown,
 * so nothing moves in retail. */
void battle_camera_update_real_coord_animation(void) {
    if (g_battle_camera_real_coord_countdown != 0) {
        g_battle_camera_real_coord_countdown--;
        g_battle_camera_current_real_coords.vx += g_battle_camera_real_coord_delta.vx;
        g_battle_camera_current_real_coords.vy += g_battle_camera_real_coord_delta.vy;
        g_battle_camera_current_real_coords.vz += g_battle_camera_real_coord_delta.vz;
    }
}
