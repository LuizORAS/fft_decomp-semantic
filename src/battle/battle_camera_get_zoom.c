#include "fft/battle.h"

/* Return the camera's three zoom scales (g_battle_camera_zoom, ONE = 1.0): the effect camera's zoom track,
 * the effect scripts' camera copy and the event camera's script variables read them. */
VECTOR* battle_camera_get_zoom(void) {
    return &g_battle_camera_zoom;
}
