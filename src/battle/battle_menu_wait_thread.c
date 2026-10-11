#include "fft/battle.h"
#include "psx/types.h"

/* Menu-opening thread 6: the Wait menu. */
void battle_menu_wait_thread(void) {
    world_menu_entry_t* menu;

    menu = (world_menu_entry_t*)battle_thread_get_current_parameter_1();
    g_battle_menu_wait_row_actions[0] = -2;
    g_battle_menu_wait_row_actions[1] = -2;
    battle_menu_confirm_action_silently(menu);
    battle_thread_exit_current();
}
