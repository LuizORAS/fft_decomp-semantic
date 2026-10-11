#include "fft/battle.h"

/* Saves the camera position for battle_camera_restore_current_real_coords (effect
 * script opcodes 09 and 0a). */
void battle_camera_copy_current_real_coords(void) {
    copyVector(&g_battle_camera_saved_real_coords, &g_battle_camera_current_real_coords);
}
