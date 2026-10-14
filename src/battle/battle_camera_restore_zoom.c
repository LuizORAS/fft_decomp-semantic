#include "fft/battle.h"

/* Put back the zoom battle_camera_save_zoom kept (effect script opcode 24). */
void battle_camera_restore_zoom(void) {
    g_battle_camera_zoom.vx = g_battle_camera_saved_zoom.vx;
    g_battle_camera_zoom.vy = g_battle_camera_saved_zoom.vy;
    g_battle_camera_zoom.vz = g_battle_camera_saved_zoom.vz;
}
