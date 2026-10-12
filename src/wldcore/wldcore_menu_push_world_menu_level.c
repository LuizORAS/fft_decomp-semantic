#include "fft/wldcore.h"
#include "psx/types.h"

/* Open WORLD's menu over the map (Triangle or Start on the map cursor): world_menu_open_entry_window
 * with menu 4 at (-0x30, -0x38), the war funds in the HUD value, and the WORLD menu level
 * (WLDCORE_MENU_LEVEL_WORLD_MENU) pushed to wait for it, with every button kept from the WORLD menu
 * frame until it starts. A fade in progress runs 4 frames longer, with the animation frozen. */
void wldcore_menu_push_world_menu_level(void) {
    s32 one;
    u32 flags;
    s32 depth;
    s32* counter;

    g_wldcore_menu_blocked_buttons = -1;
    world_menu_open_entry_window(4, -0x30, -0x38);
    one = 1;
    g_wldcore_context_value_display_mode = one;
    g_wldcore_displayed_numeric_value = world_script_get_variable(EVENT_SCRIPT_VAR_WAR_FUNDS);
    depth = g_wldcore_menu_stack_depth;
    flags = g_main_system_flags;
    g_wldcore_menu_stack_records_next[depth].world_menu.delay = 3;
    g_wldcore_menu_stack_records_next[depth].world_menu.pending = 0;
    g_main_system_flags = flags & ~MAIN_SYSTEM_FLAG_SCREEN_CLEARED;
    if ((flags & MAIN_SYSTEM_FLAG_FADING) != 0) {
        g_main_system_flags = (flags & ~MAIN_SYSTEM_FLAG_SCREEN_CLEARED) | MAIN_SYSTEM_FLAG_FREEZE_ANIMATION;
        counter = &g_wldcore_screen_fade_state.elapsed;
        *counter = *counter - 4;
    }
    {
        s32 last;
        last = g_wldcore_menu_stack_depth;
        g_wldcore_menu_stack_types[last + 1] = one;
        g_wldcore_menu_stack_depth = last + 1;
    }
}
