#include "fft/battle.h"

/* No retail code calls it. */
void camera_set_offset_screen_coords(VECTOR* coords) {
    copyVector(&g_battle_offset_screen_coords, coords);
}
