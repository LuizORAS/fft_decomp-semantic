#include "fft/battle_camera.h"

/* Let the directional buttons pan the camera over the map (battle_camera_update_map_pan) instead
 * of the camera following the cursor. */
void battle_camera_enable_map_pan(void) {
    g_battle_state_camera_pan_enabled = 1;
}
