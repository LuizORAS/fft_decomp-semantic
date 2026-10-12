#include "fft/world.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_init_subsystem_pointers. */
void world_menu_init_subsystem_pointers(void) {
    g_world_threads = g_world_thread_contexts;
    g_world_text_glyph_bitmap_data = g_world_text_glyph_bitmaps;
    g_world_menu_glyph_image = g_world_menu_glyph_image_data;
    g_world_event_block = (const u8*)g_main_script_event_block;
    g_world_script_variables = g_main_script_variables;
    g_world_menu_thread_menu_data = g_world_menu_idle_action_entries;
    g_world_help_text_id_tables_ptr = -1;
    g_world_unit_graphics_load_pending = 0;
    g_world_script_finish_check_pending = 0;
    g_world_script_frame_result_override = 0;
}
