#include "fft/wldcore.h"

/* Hide the map for a window that covers it, or show it again.
 *
 * With `capture` set it draws one whole frame (map, routes, dots, HUD value, date, location label,
 * objects), captures it as the backdrop, then hides the map frame and root menu frame windows and,
 * saving the system flags, the routes, objects, HUD value, date and location label, so only the
 * backdrop and the new windows are drawn. With `capture` 0 it shows both windows again and restores
 * the saved flags with MAIN_SYSTEM_FLAG_DRAW_MAP. */
void wldcore_gfx_toggle_captured_world_frame(s32 capture) {
    RECT display_rect;
    s32 buffer_index;

    if (capture == 0) {
        s32 map_window_index;
        s32 menu_window_index;

        map_window_index = g_wldcore_map_window_index;
        menu_window_index = g_wldcore_menu_stack_types[0];
        g_wldcore_window_records[map_window_index].flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_main_system_flags = g_wldcore_saved_system_flags | MAIN_SYSTEM_FLAG_DRAW_MAP;
        g_wldcore_window_records[menu_window_index].flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        return;
    }

    DrawSync(0);
    VSync(0);
    GsSwapDispBuff();
    buffer_index = GsGetActiveBuff();
    g_main_gfx_active_buffer_index = buffer_index;
    GsSetWorkBase(&g_wldcore_gfx_world_primitive_buffers + buffer_index * 0xE000);
    GsClearOt(0, 0, &g_wldcore_gfx_aux_ordering_tables[g_main_gfx_active_buffer_index]);
    GsClearOt(0, 0, &g_wldcore_gfx_ordering_tables[g_main_gfx_active_buffer_index]);
    wldcore_system_switch_to_stack((void*)0x1F8003FC);
    wldcore_map_project_and_cull_tiles();
    wldcore_gfx_draw_projected_map_tiles(&g_wldcore_gfx_aux_ordering_tables[g_main_gfx_active_buffer_index]);
    wldcore_map_draw_visible_routes(&g_wldcore_gfx_aux_ordering_tables[g_main_gfx_active_buffer_index]);
    wldcore_map_project_and_cull_dots();
    wldcore_map_update_camera_direction_and_projection();
    wldcore_gfx_draw_context_value_display(&g_wldcore_gfx_ordering_tables[g_main_gfx_active_buffer_index]);
    wldcore_gfx_draw_calendar_date(&g_wldcore_gfx_ordering_tables[g_main_gfx_active_buffer_index]);
    wldcore_map_draw_location_label(&g_wldcore_gfx_ordering_tables[g_main_gfx_active_buffer_index]);
    wldcore_gfx_dispatch_display_object_list(&g_wldcore_gfx_aux_ordering_tables[g_main_gfx_active_buffer_index],
        g_wldcore_window_aux_render_object_queue, g_wldcore_window_aux_render_object_count);
    wldcore_gfx_dispatch_display_object_list(&g_wldcore_gfx_ordering_tables[g_main_gfx_active_buffer_index],
        g_wldcore_window_render_object_queue, g_wldcore_window_render_object_count);
    wldcore_system_restore_previous_stack();
    GsSortClear(0, 0, 0, &g_wldcore_gfx_aux_ordering_tables[g_main_gfx_active_buffer_index]);
    GsDrawOt(&g_wldcore_gfx_aux_ordering_tables[g_main_gfx_active_buffer_index]);
    GsDrawOt(&g_wldcore_gfx_ordering_tables[g_main_gfx_active_buffer_index]);
    DrawSync(0);
    wldcore_gfx_set_display_rect(g_main_gfx_active_buffer_index, &display_rect.x);
    StoreImage(&display_rect, g_wldcore_scratch_buffer);
    DrawSync(0);
    g_wldcore_loaded_background_set = -1;
    GsClearOt(0, 0, &g_wldcore_gfx_ordering_tables[g_main_gfx_active_buffer_index]);
    wldcore_menu_run_world_frame(&g_wldcore_gfx_ordering_tables[g_main_gfx_active_buffer_index]);
    {
        s32 map_window_index;
        s32 menu_window_index;
        u32 saved_system_flags;

        saved_system_flags = g_main_system_flags;
        menu_window_index = g_wldcore_menu_stack_types[0];
        map_window_index = g_wldcore_map_window_index;
        g_wldcore_saved_system_flags = saved_system_flags;
        g_wldcore_window_records[map_window_index].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_main_system_flags = saved_system_flags | MAIN_SYSTEM_FLAG_HIDE_MAP_ROUTES
            | MAIN_SYSTEM_FLAG_HIDE_DISPLAY_OBJECTS | MAIN_SYSTEM_FLAG_HIDE_CONTEXT_VALUE
            | MAIN_SYSTEM_FLAG_HIDE_CALENDAR_DATE | MAIN_SYSTEM_FLAG_HIDE_LOCATION_LABEL;
        g_wldcore_window_records[menu_window_index].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
        wldcore_gfx_dispatch_display_object_list(&g_wldcore_gfx_ordering_tables[g_main_gfx_active_buffer_index],
            g_wldcore_window_render_object_queue, g_wldcore_window_render_object_count);
    }
    GsDrawOt(&g_wldcore_gfx_ordering_tables[g_main_gfx_active_buffer_index]);
    DrawSync(0);
}
