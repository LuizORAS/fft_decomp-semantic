#include "fft/battle.h"
#include "psx/types.h"

/* Queues companion executable 8 for loading without waiting for the current
 * request to drain (compare battle_menu_request_open_companion_executable, which blocks).
 * The poll at 0x8013d19c drives it to completion. */
void battle_menu_request_open_small(void) {
    g_battle_menu_companion_executable_request_index = 8;
}
