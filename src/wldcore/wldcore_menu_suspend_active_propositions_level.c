#include "fft/wldcore.h"

/* Suspend handler of the Job section's Advancing list (level ACTIVE_PROPOSITIONS): its cursor window
 * takes CLUT 10, the three column-header windows CLUT 6 and the list's text CLUT 2.
 * wldcore_list_handle_active_propositions_input calls it before Select's help. */
void wldcore_menu_suspend_active_propositions_level(wldcore_menu_proposition_list_level_t* level) {
    g_wldcore_window_records[level->cursor_window].palette = 10;
    g_wldcore_window_records[level->title_window].palette = g_wldcore_window_records[level->count_window].palette
        = g_wldcore_window_records[level->days_window].palette = 6;
    g_wldcore_window_render_records[level->render_index].palette = 2;
}
