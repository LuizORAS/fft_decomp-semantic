#include "fft/battle.h"
#include "fft/thread.h"

/* Thread of the Wait row: replace the entry's row actions (WAIT_YES_NO in the table) with -2 and
 * confirm it at once, without a sound. */
void battle_menu_wait_thread(void) {
    world_menu_entry_t* menu;

    menu = (world_menu_entry_t*)battle_thread_get_current_parameter_1();
    g_battle_menu_wait_row_actions[0] = -2;
    g_battle_menu_wait_row_actions[1] = -2;
    battle_menu_confirm_action_silently(menu);
    thread_exit_current();
}
