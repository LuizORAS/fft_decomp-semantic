#include "fft/battle_camera.h"
#include "fft/thread.h"

/* Yields the thread at least once, then until the camera stops moving. WORLD's copy tests BATTLE's
 * camera through battle_camera_is_active (0x8008e17c), an address that holds WLDCORE's
 * wldcore_map_find_best_route_path while WORLD is loaded; nothing reaches WORLD's event interpreter,
 * so it never runs (QUIRKS.md). */
void camera_wait_until_idle(void) {
    do {
        thread_yield();
    } while (battle_camera_is_active() != 0);
}
