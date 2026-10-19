#include "fft/wldcore.h"

/* Resume handler of a location's menu: its text render record (the level's 32-bit value 4; -1 when it has
 * none) goes back to CLUT 0. */
void wldcore_menu_resume_location_menu_level(wldcore_menu_stack_record_t* level) {
    if (level->location_list.extra_render != -1) {
        g_wldcore_window_render_records[level->location_list.extra_render].palette = 0;
    }
}
