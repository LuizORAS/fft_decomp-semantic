#include "fft/world.h"
#include "psx/types.h"

/* Starts menu thread 8 on world_menu_display_triangle_selection with the g_world_menu_system_entries menu
 * data when threads 7 and 8 are idle and g_world_menu_overlay_state is clear. */
void world_menu_start_mini_menu_display_thread(void) {
    if (world_thread_is_running_asm(NATIVE_THREAD_SLOT_MENU) != 0) {
        return;
    }
    if (world_thread_is_running_asm(NATIVE_THREAD_SLOT_MENU_CHILD) != 0) {
        return;
    }
    if (g_world_menu_overlay_state != 0) {
        return;
    }
    g_world_menu_current_id = BATTLE_MENU_ID_MINI_MENU;
    g_world_menu_thread_menu_data = g_world_menu_system_entries;
    world_thread_start(NATIVE_THREAD_SLOT_MENU, world_menu_display_triangle_selection);
    world_thread_set_parameters(
        NATIVE_THREAD_SLOT_MENU, (s32)&g_world_menu_thread_menu_data[BATTLE_MENU_ID_MINI_MENU], 0, 0);
    g_world_thread_contexts[NATIVE_THREAD_SLOT_MENU].task_id = NATIVE_THREAD_TASK_MINI_MENU;
}
