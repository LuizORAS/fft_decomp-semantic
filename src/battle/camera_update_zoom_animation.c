#include "fft/battle_camera.h"

/* Advances the zoom move by one frame while its countdown runs. Only the uncalled
 * camera_set_zoom_delta_* routines start the countdown, so nothing moves
 * in retail. */
void camera_update_zoom_animation(void) {
    if (g_camera_zoom_countdown != 0) {
        g_camera_zoom_countdown--;
        g_camera_zoom.vx += g_camera_zoom_delta.vx;
        g_camera_zoom.vy += g_camera_zoom_delta.vy;
        g_camera_zoom.vz += g_camera_zoom_delta.vz;
    }
}
