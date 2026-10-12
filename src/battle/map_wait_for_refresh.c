#include "fft/battle_map.h"
#include "fft/battle_thread.h"

/* Yield until g_map_refresh_pending is clear:
 * script_process_pending_requests zeroes it when the mesh variant load it
 * requests has finished. */
void map_wait_for_refresh(void) {
    if (g_map_refresh_pending != 0) {
        do {
            battle_thread_yield();
        } while (g_map_refresh_pending != 0);
    }
}
