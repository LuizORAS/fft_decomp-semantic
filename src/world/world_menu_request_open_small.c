#include "fft/menu.h"
#include "fft/world.h"

/* Queues the SMALL companion overlay for loading without waiting for the
 * current request to drain (menu_request_open_companion_executable
 * blocks). */
void world_menu_request_open_small(void) {
    g_menu_companion_executable_request_index = COMPANION_OVERLAY_SMALL;
}
