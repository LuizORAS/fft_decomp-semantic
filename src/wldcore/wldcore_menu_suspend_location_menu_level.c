#include "fft/wldcore.h"
#include "psx/types.h"

/* Suspend handler of a location's menu (level LOCATION_MENU): its text render record (the level's
 * word 4; -1 when it has none) takes CLUT 2. wldcore_menu_step_location_menu_level calls it before it
 * opens a child level. */
void wldcore_menu_suspend_location_menu_level(s32* level) {
    s32 index;

    index = level[4];
    if (index != -1) {
        g_wldcore_window_render_records[index].palette = 2;
    }
}
