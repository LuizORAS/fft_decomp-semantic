#include "fft/battle.h"
#include "fft/battle_camera.h"

/* Set the screen offset (g_battle_offset_screen_coords). No retail code calls it. */
void battle_camera_set_offset_screen_coords(const VECTOR* coords) {
    copyVector(&g_battle_offset_screen_coords, coords);
}
