#include "fft/battle.h"
#include "fft/battle_map.h"

/* Yield until g_battle_map_refresh_pending is clear:
 * battle_script_process_pending_requests zeroes it when the mesh variant load it
 * requests has finished. */
void battle_map_wait_for_refresh(void) {
    if (g_battle_map_refresh_pending != 0) {
        do {
            battle_thread_yield();
        } while (g_battle_map_refresh_pending != 0);
    }
}
