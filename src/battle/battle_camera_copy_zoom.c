#include "fft/battle.h"

/* Saves the camera zoom for battle_camera_restore_zoom (effect script opcodes 0f
 * and 24). */
void battle_camera_copy_zoom(void) {
    copyVector(&g_battle_camera_zoom_backup, &g_battle_camera_zoom);
}
