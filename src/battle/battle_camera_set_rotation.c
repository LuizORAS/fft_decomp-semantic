#include "fft/battle.h"

void battle_camera_set_rotation(const SVECTOR* rotation) {
    g_battle_camera_angles.vx = rotation->vx;
    g_battle_camera_angles.vy = rotation->vy;
    g_battle_camera_angles.vz = rotation->vz;
}
