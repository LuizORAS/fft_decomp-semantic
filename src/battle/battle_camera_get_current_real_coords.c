#include "fft/battle_camera.h"

VECTOR* battle_camera_get_current_real_coords(void) {
    return &g_battle_camera_current_real_coords;
}
