#include "fft/world.h"
#include "psx/types.h"

/* Return 1 while a WORLD menu screen is busy: a help screen is up (g_world_grid_menu_id), a menu
 * runs in thread slot 15 or 6, or a text task is active. The formation grid, the shop lists and name
 * entry test it each frame. Wider than world_menu_is_busy, which tests only the task and slot 6. */
s32 world_menu_is_screen_busy(void) {
    s32 result = 0;

    if (g_world_grid_menu_id != MENU_GRID_HELP_NONE || world_thread_is_running(WORLD_THREAD_SLOT_UPPER_MENU) != 0
        || g_world_thread_task_active != 0 || world_thread_is_running(NATIVE_SCREEN_THREAD_SLOT_MENU) != 0) {
        result = 1;
    }
    return result;
}
