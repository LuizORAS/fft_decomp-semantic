#include "fft/battle.h"
#include "fft/battle_camera.h"

/* No retail code calls it. */
VECTOR* camera_get_offset_screen_coords(void) {
    return &g_battle_offset_screen_coords;
}
