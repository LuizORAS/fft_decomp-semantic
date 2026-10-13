#include "fft/event_small.h"
#include "fft/menu.h"
#include "fft/world.h"
#include "psx/types.h"

/* Drive the pending BIN load and, once it completes, run the SMALL.OUT entry.
 *
 * Only request 8 (menu_request_open_small) places a file at 0x801d7000:
 * SMALL.OUT, at overlay offset 0x18000. */
s32 world_menu_poll_small_load(void) {
    if (g_menu_companion_executable_request_index == 0) {
        return 0;
    }
    world_menu_open_companion_executable();
    if (g_menu_companion_executable_request_index != 0) {
        return 1;
    }
    small_text_init_battle_pointers();
    return 0;
}
