#include "fft/battle.h"

/* Saves the camera angles for battle_camera_restore_rotation (effect script
 * opcodes 0c and 0d). */
void battle_camera_copy_rotation(void) {
    g_battle_camera_rotation_backup.vx = g_battle_camera_render_state.vx;
    g_battle_camera_rotation_backup.vy = g_battle_camera_render_state.vy;
    g_battle_camera_rotation_backup.vz = g_battle_camera_render_state.vz;
}
