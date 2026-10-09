#include "fft/battle.h"

void battle_camera_set_rotation(const SVECTOR* rotation) {
    g_battle_camera_render_state.vx = rotation->vx;
    g_battle_camera_render_state.vy = rotation->vy;
    g_battle_camera_render_state.vz = rotation->vz;
}
