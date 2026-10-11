#include "fft/battle.h"

/* No retail code calls it. */
void battle_camera_restore_offset_screen_coords(void) {
    copyVector(&g_battle_offset_screen_coords, &g_battle_camera_offset_screen_coords);
}
