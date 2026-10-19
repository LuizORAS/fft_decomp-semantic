#include "fft/wldcore.h"
#include "psx/types.h"

/* Run the top menu level's resume handler (g_wldcore_menu_resume_handlers) on its record once the
 * level above it has closed: the handler undoes what the suspend handler did and shows the level's
 * prompt again. */
void wldcore_menu_dispatch_resume_handler(void) {
    s32 index;

    index = g_wldcore_menu_stack_depth;
    g_wldcore_menu_resume_handlers[g_wldcore_menu_stack_types[index]](&g_wldcore_menu_stack_records[index]);
}
