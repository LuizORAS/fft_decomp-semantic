#include "fft/wldcore.h"

/* Draw a frame with the live 3D map, for frames where something on the map moves
 * (MAIN_SYSTEM_FLAG_DRAW_MAP, cleared at the end). The tiles, routes, path animation and map-side
 * objects go into the auxiliary ordering table; the travel step, the camera, the menus, the HUD
 * value, the date, the location label and the window objects into the main one. After the pad read
 * and VSync it swaps buffers and draws the auxiliary table, then the main one. Sibling of
 * wldcore_gfx_draw_world_frame. */
void wldcore_gfx_draw_world_frame_with_map(void) {
    GsClearOt(0, 0, &g_wldcore_gfx_aux_ordering_tables[g_active_graphics_buffer_index]);
    GsClearOt(0, 0, &g_wldcore_gfx_ordering_tables[g_active_graphics_buffer_index]);
    wldcore_gfx_draw_transition_screen(&g_wldcore_gfx_ordering_tables[g_active_graphics_buffer_index]);
    wldcore_system_switch_to_stack((void*)0x1F8003FC);
    wldcore_map_project_and_cull_tiles();
    wldcore_gfx_draw_projected_map_tiles(&g_wldcore_gfx_aux_ordering_tables[g_active_graphics_buffer_index]);
    wldcore_system_restore_previous_stack();
    wldcore_map_step_route_travel();
    wldcore_system_switch_to_stack((void*)0x1F8003FC);
    wldcore_map_draw_visible_routes(&g_wldcore_gfx_aux_ordering_tables[g_active_graphics_buffer_index]);
    wldcore_map_draw_path_animation(&g_wldcore_gfx_aux_ordering_tables[g_active_graphics_buffer_index]);
    wldcore_map_project_and_cull_dots();
    wldcore_map_update_camera_direction_and_projection();
    wldcore_system_restore_previous_stack();
    wldcore_menu_run_world_frame(&g_wldcore_gfx_ordering_tables[g_active_graphics_buffer_index]);
    wldcore_system_switch_to_stack((void*)0x1F8003FC);
    wldcore_gfx_draw_context_value_display(&g_wldcore_gfx_ordering_tables[g_active_graphics_buffer_index]);
    wldcore_gfx_draw_calendar_date(&g_wldcore_gfx_ordering_tables[g_active_graphics_buffer_index]);
    wldcore_map_draw_location_label(&g_wldcore_gfx_ordering_tables[g_active_graphics_buffer_index]);
    wldcore_gfx_dispatch_display_object_list(&g_wldcore_gfx_aux_ordering_tables[g_active_graphics_buffer_index],
        g_wldcore_window_aux_render_object_queue, g_wldcore_window_aux_render_object_count);
    wldcore_gfx_dispatch_display_object_list(&g_wldcore_gfx_ordering_tables[g_active_graphics_buffer_index],
        g_wldcore_window_render_object_queue, g_wldcore_window_render_object_count);
    wldcore_system_restore_previous_stack();
    DrawSync(0);
    wldcore_input_read_pad();
    VSync(0);
    GsSwapDispBuff();
    GsSortClear(0, 0, 0, &g_wldcore_gfx_aux_ordering_tables[g_active_graphics_buffer_index]);
    GsDrawOt(&g_wldcore_gfx_aux_ordering_tables[g_active_graphics_buffer_index]);
    GsDrawOt(&g_wldcore_gfx_ordering_tables[g_active_graphics_buffer_index]);
    g_main_system_flags &= ~MAIN_SYSTEM_FLAG_DRAW_MAP;
}
