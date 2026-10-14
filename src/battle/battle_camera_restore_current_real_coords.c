#include "fft/battle.h"

/* Put back the camera position battle_camera_save_current_real_coords kept (effect script opcode 0a). */
void battle_camera_restore_current_real_coords(void) {
    g_battle_camera_current_real_coords.vx = g_battle_camera_saved_real_coords.vx;
    g_battle_camera_current_real_coords.vy = g_battle_camera_saved_real_coords.vy;
    g_battle_camera_current_real_coords.vz = g_battle_camera_saved_real_coords.vz;
}
