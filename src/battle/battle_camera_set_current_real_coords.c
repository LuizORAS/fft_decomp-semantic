#include "fft/battle_camera.h"

/* Set the camera position (20.12): effect script opcode 08, the effect camera's position track and the
 * event camera's script variables. */
void battle_camera_set_current_real_coords(const VECTOR* coords) {
    copyVector(&g_battle_camera_current_real_coords, coords);
}
