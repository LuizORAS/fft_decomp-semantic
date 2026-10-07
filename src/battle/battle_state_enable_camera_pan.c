#include "fft/battle.h"
#include "psx/types.h"

/* Let the directional buttons pan the camera over the map instead of the camera following
 * the cursor. */
void battle_state_enable_camera_pan(void) {
    g_battle_state_camera_pan_enabled = 1;
}
