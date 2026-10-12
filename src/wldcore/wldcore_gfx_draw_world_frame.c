#include "fft/wldcore.h"

/* Draw a frame over the captured map backdrop, for frames where nothing on the map moves: the
 * menus, HUD value, date, location label, window objects and screen fade boxes. After the pad read
 * and VSync it swaps buffers and, unless MAIN_SYSTEM_FLAG_NO_BACKGROUND_IMAGE is set, uploads the
 * backdrop (g_wldcore_scratch_buffer) into the buffer about to be drawn. */
void wldcore_gfx_draw_world_frame(void) {
    RECT rect;

    GsClearOt(0, 0, &g_wldcore_gfx_ordering_tables[g_active_graphics_buffer_index]);
    DrawSync(0);
    wldcore_draw_close_indicator_if_pending(&g_wldcore_gfx_ordering_tables[g_active_graphics_buffer_index]);
    wldcore_menu_run_world_frame(&g_wldcore_gfx_ordering_tables[g_active_graphics_buffer_index]);
    wldcore_switch_to_stack((void*)0x1F8003FC);
    wldcore_gfx_draw_context_value_display(&g_wldcore_gfx_ordering_tables[g_active_graphics_buffer_index]);
    wldcore_gfx_draw_calendar_date(&g_wldcore_gfx_ordering_tables[g_active_graphics_buffer_index]);
    wldcore_map_draw_location_label(&g_wldcore_gfx_ordering_tables[g_active_graphics_buffer_index]);
    wldcore_dispatch_display_object_list(&g_wldcore_gfx_ordering_tables[g_active_graphics_buffer_index],
        g_wldcore_window_render_object_queue, g_wldcore_window_render_object_count);
    wldcore_fade_draw_screen_overlays();
    wldcore_restore_previous_stack();
    DrawSync(0);
    wldcore_input_read_pad();
    VSync(0);
    GsSwapDispBuff();
    if (!(g_main_system_flags & MAIN_SYSTEM_FLAG_NO_BACKGROUND_IMAGE)) {
        wldcore_gfx_set_display_rect(g_active_graphics_buffer_index == 0, &rect.x);
        LoadImage(&rect, g_wldcore_scratch_buffer);
    }
    GsDrawOt(&g_wldcore_gfx_ordering_tables[g_active_graphics_buffer_index]);
}
