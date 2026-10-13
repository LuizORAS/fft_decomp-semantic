#include "fft/event_small.h"
#include "fft/world.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_poll_small_load. */
s32 world_menu_poll_small_load(void) {
    if (g_world_menu_companion_executable_request_index == 0) {
        return 0;
    }
    world_menu_open_companion_executable();
    if (g_world_menu_companion_executable_request_index != 0) {
        return 1;
    }
    small_text_init_battle_pointers();
    return 0;
}
