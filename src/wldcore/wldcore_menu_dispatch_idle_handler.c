#include "fft/wldcore.h"

/* Run the top menu level's idle handler (g_wldcore_menu_idle_handlers, by its level type) on its
 * record. wldcore_entrypoint calls it once a frame: it is the level's per-frame step. */
void wldcore_menu_dispatch_idle_handler(void) {
    s32 index;

    index = g_wldcore_menu_stack_depth;
    g_wldcore_menu_idle_handlers[g_wldcore_menu_stack_types[index]](&g_wldcore_menu_stack_records[index]);
}
