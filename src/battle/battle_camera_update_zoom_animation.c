#include "fft/battle.h"

/* Advances the zoom move by one frame while its countdown runs. Only the uncalled
 * battle_camera_set_zoom_delta_* routines start the countdown, so nothing moves
 * in retail. */
void battle_camera_update_zoom_animation(void) {
    if (g_battle_camera_zoom_countdown != 0) {
        g_battle_camera_zoom_countdown--;
        g_battle_camera_zoom.vx += g_battle_camera_zoom_delta.vx;
        g_battle_camera_zoom.vy += g_battle_camera_zoom_delta.vy;
        g_battle_camera_zoom.vz += g_battle_camera_zoom_delta.vz;
    }
}
