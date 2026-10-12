#include "fft/battle_camera.h"

SVECTOR* battle_camera_get_rotation(void) {
    return &g_battle_camera_render_state;
}
