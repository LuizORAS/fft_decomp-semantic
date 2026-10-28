#include "fft/battle.h"
#include "fft/battle_camera.h"

/* Return the screen offset (g_battle_offset_screen_coords). No retail code calls it. */
VECTOR* battle_camera_get_offset_screen_coords(void) {
    return &g_battle_offset_screen_coords;
}
