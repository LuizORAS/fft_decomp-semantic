#include "fft/wldcore.h"
#include "psx/types.h"

/* Resume handler of the world map's root level: show the war funds in the
 * context value display, reset the level's window palette, restore the live
 * map if the captured frame is showing and run any pending script requests. */
void wldcore_menu_resume_root_level(wldcore_menu_stack_record_t* level) {
    s32 index = level->map_cursor.window_index;

    g_wldcore_context_value_display_mode = WLDCORE_VALUE_DISPLAY_FUNDS;
    g_wldcore_window_records[index].palette = 0;
    g_wldcore_displayed_numeric_value = world_script_get_variable(EVENT_SCRIPT_VAR_WAR_FUNDS);
    if (g_wldcore_window_records[g_wldcore_map_window_index].flags & WLDCORE_DISPLAY_OBJECT_HIDDEN) {
        wldcore_gfx_toggle_captured_world_frame(0);
    }
    wldcore_menu_dispatch_pending_script_requests();
}
