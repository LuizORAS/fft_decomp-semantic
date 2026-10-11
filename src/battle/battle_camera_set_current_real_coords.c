#include "fft/battle.h"

void battle_camera_set_current_real_coords(const VECTOR* coords) {
    copyVector(&g_battle_camera_current_real_coords, coords);
}
