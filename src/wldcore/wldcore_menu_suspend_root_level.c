#include "fft/wldcore.h"

/* Suspend handler of the root level (the map cursor): the cursor window takes CLUT 10 and the map
 * cursor's scroll stops. The code that opens a screen, the shop or a script request over the map calls
 * it too.
 *
 * The reset targets are struct fields (not scalar globals) so GCC 2.6.3's scheduler keeps them ordered
 * after the indexed entry store. */
void wldcore_menu_suspend_root_level(wldcore_menu_stack_record_t* level) {
    g_wldcore_window_records[level->window_index].palette = 10;
    g_wldcore_map_cursor_scroll_y.value = 0;
    g_wldcore_map_cursor_scroll_x.value = 0;
    g_wldcore_map_cursor_scroll_y.output = 0;
    g_wldcore_map_cursor_scroll_x.output = 0;
}
