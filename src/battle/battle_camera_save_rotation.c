#include "fft/battle.h"

/* Saves the camera angles for battle_camera_restore_rotation (effect script
 * opcodes 0c and 0d). */
void battle_camera_save_rotation(void) {
    g_battle_camera_saved_rotation.vx = g_battle_camera_angles.vx;
    g_battle_camera_saved_rotation.vy = g_battle_camera_angles.vy;
    g_battle_camera_saved_rotation.vz = g_battle_camera_angles.vz;
}
