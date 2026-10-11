#include "fft/battle.h"

/* Saves the camera zoom for battle_camera_restore_zoom (effect script opcodes 0f
 * and 24). */
void battle_camera_copy_zoom(void) {
    g_battle_camera_zoom_backup.vx = g_battle_camera_zoom.vx;
    g_battle_camera_zoom_backup.vy = g_battle_camera_zoom.vy;
    g_battle_camera_zoom_backup.vz = g_battle_camera_zoom.vz;
}
