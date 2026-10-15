#include "fft/battle.h"
#include "psx/types.h"

/* Return the camera to following the cursor; most state entries call it. */
void battle_state_disable_camera_pan(void) {
    g_battle_state_camera_pan_enabled = 0;
}
