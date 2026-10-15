#include "fft/battle.h"

/* Saves the screen offset for battle_camera_restore_offset_screen_coords. No
 * retail code calls either. */
void battle_camera_save_offset_screen_coords(void) {
    g_battle_camera_saved_offset_screen_coords.vx = g_battle_offset_screen_coords.vx;
    g_battle_camera_saved_offset_screen_coords.vy = g_battle_offset_screen_coords.vy;
    g_battle_camera_saved_offset_screen_coords.vz = g_battle_offset_screen_coords.vz;
}
