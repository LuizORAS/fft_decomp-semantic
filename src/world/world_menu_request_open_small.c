#include "fft/world.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_request_open_small. */
void world_menu_request_open_small(void) {
    g_world_menu_companion_executable_request_index = BATTLE_COMPANION_OVERLAY_SMALL;
}
