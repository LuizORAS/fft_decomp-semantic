#include "fft/battle_camera.h"

/* Set the camera angles: effect script opcode 0b, the effect camera's angle track and the event camera's
 * script variables. */
void battle_camera_set_rotation(const SVECTOR* rotation) {
    copyVector(&g_battle_camera_angles, rotation);
}
