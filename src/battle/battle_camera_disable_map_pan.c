#include "fft/battle_camera.h"

/* Return the camera to following the cursor; most state entries call it. */
void battle_camera_disable_map_pan(void) {
    g_battle_state_camera_pan_enabled = 0;
}
