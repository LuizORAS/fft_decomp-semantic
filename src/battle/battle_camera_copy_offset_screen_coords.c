#include "fft/battle.h"

/* Saves the screen offset for battle_camera_restore_offset_screen_coords. No
 * retail code calls either. */
void battle_camera_copy_offset_screen_coords(void) {
    copyVector(&g_battle_camera_offset_screen_coords, &g_battle_offset_screen_coords);
}
