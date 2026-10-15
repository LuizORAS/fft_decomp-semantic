#include "fft/wldcore.h"
#include "psx/types.h"

/* Suspend handler of the Job section's Advancing list (level ACTIVE_PROPOSITIONS): its cursor window
 * takes CLUT 10, the three column-header windows CLUT 6 and the list's text CLUT 2.
 * wldcore_list_handle_active_propositions_input calls it before Select's help. */
void wldcore_menu_suspend_active_propositions_level(s32* param) {
    g_wldcore_window_records[param[0]].palette = 10;
    g_wldcore_window_records[param[1]].palette = g_wldcore_window_records[param[2]].palette
        = g_wldcore_window_records[param[3]].palette = 6;
    g_wldcore_window_render_records[param[4]].palette = 2;
}
