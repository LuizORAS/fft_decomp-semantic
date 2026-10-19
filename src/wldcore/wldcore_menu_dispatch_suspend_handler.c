#include "fft/wldcore.h"
#include "psx/types.h"

/* Run the top menu level's suspend handler (g_wldcore_menu_suspend_handlers) on its record before
 * another level opens over it. A suspend handler dims the level (CLUT 10 on its cursor window, 6 on its
 * side windows, 2 on its text) and holds its cursor; its resume handler undoes that. */
void wldcore_menu_dispatch_suspend_handler(void) {
    s32 index;

    index = g_wldcore_menu_stack_depth;
    g_wldcore_menu_suspend_handlers[g_wldcore_menu_stack_types[index]](&g_wldcore_menu_stack_records[index]);
}
