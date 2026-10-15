#include "fft/wldcore.h"
#include "psx/types.h"

/* Resume handler of a location's menu: its text render record (the level's word 4; -1 when it has
 * none) goes back to CLUT 0. */
void wldcore_menu_resume_location_menu_level(s32* level) {
    s32 index;

    index = level[4];
    if (index != -1) {
        g_wldcore_window_render_records[index].palette = 0;
    }
}
