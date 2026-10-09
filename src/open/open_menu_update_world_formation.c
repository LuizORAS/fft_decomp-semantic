#include "fft/open.h"
#include "psx/types.h"

void open_menu_update_world_formation(void) {
    if (g_open_system_runtime_flags & OPEN_SYSTEM_RUNTIME_FLAG_WORLD_MENU_FRAME) {
        u32 blocked_buttons = g_open_menu_blocked_buttons;

        g_active_graphics_buffer_index ^= 1;
        world_formation_set_menu_allowed_buttons(~blocked_buttons);
        world_formation_run_menu_frame(
            &g_open_gfx_otags[g_active_graphics_buffer_index ^ 1][g_open_menu_formation_otag_index],
            g_open_input_current_buttons);
        g_active_graphics_buffer_index ^= 1;
    }
}
