#include "fft/world.h"
#include "psx/types.h"

void world_menu_request_open_small(void) {
    g_world_bin_load_request = BATTLE_COMPANION_OVERLAY_SMALL;
}
