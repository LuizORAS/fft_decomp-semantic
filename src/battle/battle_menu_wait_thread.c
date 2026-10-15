#include "fft/battle.h"
#include "psx/types.h"

/* Thread of the Wait row: replace the entry's row actions (WAIT_YES_NO in the table) with -2 and
 * confirm it at once, without a sound. */
void battle_menu_wait_thread(void) {
    void* menu;

    menu = (void*)battle_thread_get_current_parameter_1();
    g_battle_menu_wait_row_actions[0] = -2;
    g_battle_menu_wait_row_actions[1] = -2;
    battle_menu_confirm_action_silently(menu);
    battle_thread_exit_current();
}
