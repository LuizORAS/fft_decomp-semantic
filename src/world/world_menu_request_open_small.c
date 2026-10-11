#include "fft/world.h"
#include "psx/types.h"

/* Queues the SMALL companion overlay for loading without waiting for the
 * current request to drain (world_menu_request_open_companion_executable
 * blocks). */
void world_menu_request_open_small(void) {
    g_world_bin_load_request = COMPANION_OVERLAY_SMALL;
}
