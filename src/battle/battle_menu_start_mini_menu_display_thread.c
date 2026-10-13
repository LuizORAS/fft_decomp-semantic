#include "fft/battle_menu.h"
#include "fft/thread.h"

/* Start the mini menu in the menu thread slot, on
 * battle_menu_display_triangle_selection with the mini menu's entry, unless the
 * menu or its child thread is running or the menus are suspended. */
void battle_menu_start_mini_menu_display_thread(void) {
    if (battle_thread_is_running_asm(NATIVE_THREAD_SLOT_MENU) != 0) {
        return;
    }
    if (battle_thread_is_running_asm(NATIVE_THREAD_SLOT_MENU_CHILD) != 0) {
        return;
    }
    if (g_battle_menu_suspend_state != 0) {
        return;
    }
    g_battle_menu_current_id = BATTLE_MENU_ID_MINI_MENU;
    g_battle_menu_thread_menu_data = g_battle_menu_idle_action_entries;
    thread_start(NATIVE_THREAD_SLOT_MENU, battle_menu_display_triangle_selection);
    thread_set_parameters(
        NATIVE_THREAD_SLOT_MENU, (s32)&g_battle_menu_thread_menu_data[BATTLE_MENU_ID_MINI_MENU], 0, 0);
    g_native_thread_contexts[NATIVE_THREAD_SLOT_MENU].task_id = NATIVE_THREAD_TASK_MINI_MENU;
}
