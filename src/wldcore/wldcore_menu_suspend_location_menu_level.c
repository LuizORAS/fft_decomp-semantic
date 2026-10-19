#include "fft/wldcore.h"

/* Suspend handler of a location's menu (level LOCATION_MENU): its text render record (the level's 32-bit value
 * 4; -1 when it has none) takes CLUT 2. wldcore_menu_step_location_menu_level calls it before it
 * opens a child level. */
void wldcore_menu_suspend_location_menu_level(wldcore_menu_slot_set_t* slots) {
    if (slots->render_b != -1) {
        g_wldcore_window_render_records[slots->render_b].palette = 2;
    }
}
