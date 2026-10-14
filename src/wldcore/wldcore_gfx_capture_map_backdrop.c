#include "fft/wldcore.h"

/* Capture the map as the still backdrop: draw the tiles, routes, dots and the map-side display
 * objects into the auxiliary ordering table, store the drawn frame into g_wldcore_scratch_buffer
 * (which drops the loaded background set) and draw the menus, HUD value, date and location label
 * over it. wldcore_gfx_draw_world_frame uploads the backdrop every frame until the map moves again.
 * Runs when the map stops moving (wldcore_gfx_update_map_backdrop), as a visit starts
 * (wldcore_system_init_screen) and when WORLD's menu returns with a destination. */
void wldcore_gfx_capture_map_backdrop(void) {
    RECT rect;
    s32 count;

    DrawSync(0);
    VSync(0);
    GsSwapDispBuff();
    count = GsGetActiveBuff();
    g_active_graphics_buffer_index = count;
    GsSetWorkBase(&g_wldcore_gfx_world_primitive_buffers + count * 0xE000);
    GsClearOt(0, 0, &g_wldcore_gfx_aux_ordering_tables[g_active_graphics_buffer_index]);
    wldcore_gfx_draw_transition_screen(&g_wldcore_gfx_aux_ordering_tables[g_active_graphics_buffer_index]);
    wldcore_system_switch_to_stack((void*)0x1F8003FC);
    wldcore_gfx_draw_projected_map_tiles(&g_wldcore_gfx_aux_ordering_tables[g_active_graphics_buffer_index]);
    wldcore_map_draw_visible_routes(&g_wldcore_gfx_aux_ordering_tables[g_active_graphics_buffer_index]);
    wldcore_map_project_and_cull_dots();
    wldcore_gfx_dispatch_display_object_list(&g_wldcore_gfx_aux_ordering_tables[g_active_graphics_buffer_index],
        g_wldcore_window_aux_render_object_queue, g_wldcore_window_aux_render_object_count);
    wldcore_system_restore_previous_stack();
    GsSortClear(0, 0, 0, &g_wldcore_gfx_aux_ordering_tables[g_active_graphics_buffer_index]);
    GsDrawOt(&g_wldcore_gfx_aux_ordering_tables[g_active_graphics_buffer_index]);
    DrawSync(0);
    wldcore_gfx_set_display_rect(g_active_graphics_buffer_index, &rect.x);
    StoreImage(&rect, g_wldcore_scratch_buffer);
    DrawSync(0);
    g_wldcore_loaded_background_set = -1;
    GsClearOt(0, 0, &g_wldcore_gfx_ordering_tables[g_active_graphics_buffer_index]);
    wldcore_map_update_camera_direction_and_projection();
    wldcore_menu_run_world_frame(&g_wldcore_gfx_ordering_tables[g_active_graphics_buffer_index]);
    wldcore_gfx_draw_context_value_display(&g_wldcore_gfx_ordering_tables[g_active_graphics_buffer_index]);
    wldcore_gfx_draw_calendar_date(&g_wldcore_gfx_ordering_tables[g_active_graphics_buffer_index]);
    wldcore_map_draw_location_label(&g_wldcore_gfx_ordering_tables[g_active_graphics_buffer_index]);
    wldcore_gfx_dispatch_display_object_list(&g_wldcore_gfx_ordering_tables[g_active_graphics_buffer_index],
        g_wldcore_window_render_object_queue, g_wldcore_window_render_object_count);
    GsDrawOt(&g_wldcore_gfx_ordering_tables[g_active_graphics_buffer_index]);
    DrawSync(0);
}
