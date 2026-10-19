#include "fft/battle_map.h"
#include "fft/thread.h"

/* Yield until g_map_refresh_pending is clear: battle_script_process_pending_requests zeroes it when
 * the mesh variant load it requests has finished. WORLD's copy waits on g_world_map_refresh_pending. */
void map_wait_for_refresh(void) {
    if (g_map_refresh_pending != 0) {
        do {
            thread_yield();
        } while (g_map_refresh_pending != 0);
    }
}
