#include "fft/battle.h"
#include "fft/battle_camera.h"

/* Put back the screen offset battle_camera_save_offset_screen_coords kept. No retail code calls it. */
void battle_camera_restore_offset_screen_coords(void) {
    copyVector(&g_battle_offset_screen_coords, &g_battle_camera_saved_offset_screen_coords);
}
