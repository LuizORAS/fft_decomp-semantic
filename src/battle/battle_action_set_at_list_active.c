#include "fft/battle_action.h"

/* Show the map cursor and its tile glow: the flag is read only by
 * battle_target_calculate_tile_coords_and_glow_from_at_list. */
void battle_action_set_at_list_active(void) {
    g_battle_action_at_list_active = 1;
}
