#include "fft/battle.h"

/* Return the camera position (20.12, g_battle_camera_current_real_coords): the effect camera's position
 * track, the effect scripts' camera copy and the event camera's script variables read it. */
VECTOR* battle_camera_get_current_real_coords(void) {
    return &g_battle_camera_current_real_coords;
}
