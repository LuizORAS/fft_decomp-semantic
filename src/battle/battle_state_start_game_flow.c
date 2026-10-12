#include "fft/battle.h"

/* Let the held pad directions pan the camera across the map
 * (camera_update_map_pan); the camera stops following the cursor tile. */
void battle_state_start_game_flow(void) {
    g_battle_state_game_flow_running = 1;
}
