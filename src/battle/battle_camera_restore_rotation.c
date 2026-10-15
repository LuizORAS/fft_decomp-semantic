#include "fft/battle.h"

/* Put back the camera angles battle_camera_save_rotation kept (effect script opcode 0d). */
void battle_camera_restore_rotation(void) {
    g_battle_camera_angles.vx = g_battle_camera_saved_rotation.vx;
    g_battle_camera_angles.vy = g_battle_camera_saved_rotation.vy;
    g_battle_camera_angles.vz = g_battle_camera_saved_rotation.vz;
}
