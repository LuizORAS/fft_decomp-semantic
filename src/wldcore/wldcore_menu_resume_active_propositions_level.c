#include "fft/wldcore.h"

/* Resume handler of the Advancing list: its windows and text go back to CLUT 0 and the cursor to its
 * idle sequence. */
void wldcore_menu_resume_active_propositions_level(wldcore_menu_stack_record_t* level) {
    g_wldcore_window_records[level->proposition_list.cursor_window].palette
        = g_wldcore_window_records[level->proposition_list.title_window].palette
        = g_wldcore_window_records[level->proposition_list.count_window].palette
        = g_wldcore_window_records[level->proposition_list.days_window].palette
        = g_wldcore_window_render_records[level->proposition_list.render_index].palette = 0;
    g_wldcore_window_records[level->proposition_list.cursor_window].sequence = WLDCORE_ANIM_SEQUENCE_CURSOR_IDLE;
    g_wldcore_window_records[level->proposition_list.cursor_window].frame_index
        = g_wldcore_window_records[level->proposition_list.cursor_window].anim_counter = 0;
}
