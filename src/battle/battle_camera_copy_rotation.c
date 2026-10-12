#include "fft/battle_camera.h"

/* Saves the camera angles for battle_camera_restore_rotation (effect script
 * opcodes 0c and 0d). */
void battle_camera_copy_rotation(void) {
    copyVector(&g_battle_camera_rotation_backup, &g_battle_camera_render_state);
}
