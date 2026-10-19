#include "fft/wldcore.h"
#include "psx/libetc.h"

/* Draw a frame over the captured map backdrop, for frames where nothing on the map moves: the
 * menus, HUD value, date, location label, window objects and screen fade boxes. After the pad read
 * and VSync it swaps buffers and, unless MAIN_SYSTEM_FLAG_NO_BACKGROUND_IMAGE is set, uploads the
 * backdrop (g_wldcore_scratch_buffer) into the buffer about to be drawn. */
void wldcore_gfx_draw_world_frame(void) {
    RECT rect;

    GsClearOt(0, 0, &g_wldcore_gfx_ordering_tables[g_main_gfx_active_buffer_index]);
    DrawSync(0);
    wldcore_gfx_draw_transition_screen(&g_wldcore_gfx_ordering_tables[g_main_gfx_active_buffer_index]);
    wldcore_menu_run_world_frame(&g_wldcore_gfx_ordering_tables[g_main_gfx_active_buffer_index]);
    wldcore_system_switch_to_stack(getScratchAddr(255));
    wldcore_gfx_draw_context_value_display(&g_wldcore_gfx_ordering_tables[g_main_gfx_active_buffer_index]);
    wldcore_gfx_draw_calendar_date(&g_wldcore_gfx_ordering_tables[g_main_gfx_active_buffer_index]);
    wldcore_map_draw_location_label(&g_wldcore_gfx_ordering_tables[g_main_gfx_active_buffer_index]);
    wldcore_gfx_dispatch_display_object_list(&g_wldcore_gfx_ordering_tables[g_main_gfx_active_buffer_index],
        g_wldcore_window_render_object_queue, g_wldcore_window_render_object_count);
    wldcore_gfx_draw_screen_fade_overlays();
    wldcore_system_restore_previous_stack();
    DrawSync(0);
    wldcore_input_read_pad();
    VSync(0);
    GsSwapDispBuff();
    if (!(g_main_system_flags & MAIN_SYSTEM_FLAG_NO_BACKGROUND_IMAGE)) {
        wldcore_gfx_set_display_rect(g_main_gfx_active_buffer_index == 0, &rect);
        LoadImage(&rect, g_wldcore_scratch_buffer);
    }
    GsDrawOt(&g_wldcore_gfx_ordering_tables[g_main_gfx_active_buffer_index]);
}
