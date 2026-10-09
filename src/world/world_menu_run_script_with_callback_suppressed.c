#include "fft/world.h"

#define WORLD_MENU_SCRIPT_PALETTE_BYTE 0x29

void world_menu_run_script_with_callback_suppressed(void) {
    s32 (*previous_callback)(s32);

    previous_callback = g_world_menu_script_callbacks[0];
    g_world_menu_script_callbacks[0] = 0;
    g_world_shop_roster_count_menu_script[WORLD_MENU_SCRIPT_PALETTE_BYTE] = g_world_shop_party_unit_count;
    world_menu_run_script_with_palette_mode(g_world_shop_roster_count_menu_script, 0, 0);
    g_world_menu_script_callbacks[0] = previous_callback;
}
