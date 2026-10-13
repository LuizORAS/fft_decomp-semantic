#include "fft/battle_menu.h"
#include "fft/menu.h"

/* Queues companion executable 8 for loading without waiting for the current
 * request to drain (compare menu_request_open_companion_executable, which blocks).
 * The poll at 0x8013d19c drives it to completion. */
void battle_menu_request_open_small(void) {
    g_menu_companion_executable_request_index = COMPANION_OVERLAY_SMALL;
}
