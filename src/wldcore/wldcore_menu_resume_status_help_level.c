#include "fft/wldcore.h"

/* Resume handler of the status help, once a field's help text closes: clear the input lock and undo
 * the pressed cursor (the sequence after it, CLUT 0, animation restarted). */
void wldcore_menu_resume_status_help_level(s32* param) {
    s32 index = *param;
    wldcore_window_record_t* entry = &g_wldcore_window_records[index];

    g_world_menu_input_lock_mode = 0;
    entry->sequence = g_wldcore_window_records[index].sequence + 1;
    g_wldcore_window_records[index].palette = 0;
    entry->anim_counter = 0;
    g_wldcore_window_records[index].frame_index = 0;
}
