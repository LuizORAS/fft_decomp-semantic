#include "fft/battle.h"

void battle_camera_set_rotation(const SVECTOR* rotation) {
    copyVector(&g_battle_camera_render_state, rotation);
}
