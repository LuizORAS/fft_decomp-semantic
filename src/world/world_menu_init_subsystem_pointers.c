#include "fft/script.h"
#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/* Bind the WORLD scheduler, script and menu subsystems to their backing
   storage and clear the per-scene state they share. */
void world_menu_init_subsystem_pointers(void) {
    g_native_threads = g_native_thread_contexts;
    g_world_text_glyph_bitmap_data = g_world_text_glyph_bitmaps;
    g_world_menu_glyph_sheet = &g_world_menu_glyph_image;
    g_world_event_script = (u8*)g_main_script_event_block;
    g_script_variables = g_main_script_variables;
    g_world_menu_thread_menu_data = g_world_menu_system_entries;
    g_world_help_active_text_id_tables = (u16* const*)-1;
    g_world_unit_graphics_load_pending = 0;
    g_world_event_finish_check_pending = 0;
    g_script_frame_result_override = 0;
}
