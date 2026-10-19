#include "fft/menu.h"

/* Queues companion executable 8 for loading without waiting for the current
 * request to drain (compare menu_request_open_companion_executable, which blocks).
 * menu_poll_small_load drives it to completion. WORLD compiles this text too. */
void menu_request_open_small(void) {
    g_menu_companion_executable_request_index = BATTLE_COMPANION_OVERLAY_SMALL;
}
