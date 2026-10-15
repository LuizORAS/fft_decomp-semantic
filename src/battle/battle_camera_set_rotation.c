#include "fft/battle.h"

/* Set the camera angles: effect script opcode 0b, the effect camera's angle track and the event camera's
 * script variables. */
void battle_camera_set_rotation(const SVECTOR* rotation) {
    g_battle_camera_angles.vx = rotation->vx;
    g_battle_camera_angles.vy = rotation->vy;
    g_battle_camera_angles.vz = rotation->vz;
}
