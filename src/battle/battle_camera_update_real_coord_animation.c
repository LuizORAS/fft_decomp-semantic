#include "fft/battle_camera.h"

/* Advances a camera move by one frame while its countdown runs: the camera-position move here, and the
 * zoom move in battle_camera_update_zoom_animation, the same text bound to the zoom globals. Only the
 * uncalled set_real_coord_delta and set_zoom_delta routines start the countdowns, so nothing moves in
 * retail. */
void battle_camera_update_real_coord_animation(void) {
    if (g_battle_camera_real_coord_countdown != 0) {
        g_battle_camera_real_coord_countdown--;
        g_battle_camera_current_real_coords.vx += g_battle_camera_real_coord_delta.vx;
        g_battle_camera_current_real_coords.vy += g_battle_camera_real_coord_delta.vy;
        g_battle_camera_current_real_coords.vz += g_battle_camera_real_coord_delta.vz;
    }
}
