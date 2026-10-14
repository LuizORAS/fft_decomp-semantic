#include "fft/battle.h"
#include "fft/world.h"
#include "psx/types.h"

/* Yield the thread at least once, then until the camera stops moving: the WORLD twin of
 * battle_camera_wait_until_idle, which tests BATTLE's camera through battle_camera_is_active (0x8008e17c). With
 * WORLD loaded that address lies inside WLDCORE's wldcore_map_find_best_route_path, but nothing reaches WORLD's
 * event interpreter (QUIRKS.md). */
void world_camera_wait_until_idle(void) {
    do {
        world_thread_yield();
    } while (battle_camera_is_active() != 0);
}
