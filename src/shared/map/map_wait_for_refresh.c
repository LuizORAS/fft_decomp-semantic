#include "fft/battle_map.h"
#include "fft/thread.h"

/* Yield until this scene's pending map refresh clears. */
void map_wait_for_refresh(void) {
    if (g_map_refresh_pending != 0) {
        do {
            thread_yield();
        } while (g_map_refresh_pending != 0);
    }
}
