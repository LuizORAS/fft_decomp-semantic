#include "fft/battle_event.h"
#include "fft/battle_map.h"

/* Advance the map-destruction wait state from 1 (armed when a map append
 * finishes) to 2. battle_script_process_pending_requests then releases the held
 * GNS data and polls the map load, returning the state to 0 when the load ends.
 * Any other state is left alone. */
void battle_script_set_specialized_map_destroyed(void) {

    if (g_battle_map_destruction_wait_state == 1) {
        g_battle_map_destruction_wait_state = g_battle_map_destruction_wait_state + 1;
    }
}
