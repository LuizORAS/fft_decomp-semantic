#include "fft/battle.h"
#include "fft/battle_camera.h"

/* No retail code calls it. */
void camera_restore_offset_screen_coords(void) {
    copyVector(&g_battle_offset_screen_coords, &g_camera_offset_screen_coords);
}
