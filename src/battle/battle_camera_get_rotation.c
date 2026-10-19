#include "fft/battle_camera.h"

/* Return the camera angles (g_battle_camera_angles): the effect camera's angle track, the effect scripts'
 * camera copy, the event camera's script variables and battle_camera_choose_unblocked_rotation_for_tile read
 * them. */
SVECTOR* battle_camera_get_rotation(void) {
    return &g_battle_camera_angles;
}
