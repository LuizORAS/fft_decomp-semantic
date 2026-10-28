#include "fft/wldcore.h"

/* Once a frame: when the live map has just stopped drawing (MAIN_SYSTEM_FLAG_DRAW_MAP set last
 * frame and clear now), capture it as the backdrop (wldcore_gfx_capture_map_backdrop); then keep this
 * frame's flags in g_wldcore_previous_system_flags. */
void wldcore_gfx_update_map_backdrop(void) {
    if ((g_main_system_flags & MAIN_SYSTEM_FLAG_DRAW_MAP) == 0
        && (g_wldcore_previous_system_flags & MAIN_SYSTEM_FLAG_DRAW_MAP) != 0) {
        wldcore_gfx_capture_map_backdrop();
    }
    g_wldcore_previous_system_flags = g_main_system_flags;
}
