#include "fft/open.h"
#include "psx/types.h"

/* While OPEN_SYSTEM_RUNTIME_FLAG_WORLD_MENU_FRAME is set (the Music Test), run one frame of WORLD's menus
 * (world_menu_run_frame) with g_open_menu_blocked_buttons withheld, linking into entry
 * g_open_menu_world_menu_otag_index of the ordering table OPEN is building. The active buffer index is
 * flipped for the duration of the call. */
void open_menu_run_world_menu_frame(void) {
    if (g_open_system_runtime_flags & OPEN_SYSTEM_RUNTIME_FLAG_WORLD_MENU_FRAME) {
        u32 blocked_buttons = g_open_menu_blocked_buttons;

        g_main_gfx_active_buffer_index ^= 1;
        world_menu_set_allowed_buttons(~blocked_buttons);
        world_menu_run_frame(&g_open_gfx_otags[g_main_gfx_active_buffer_index ^ 1][g_open_menu_world_menu_otag_index],
            g_open_input_current_buttons);
        g_main_gfx_active_buffer_index ^= 1;
    }
}
