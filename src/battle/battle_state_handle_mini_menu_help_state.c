#include "fft/battle.h"

/* MINI_MENU_HELP: once the help window has closed, reopen the free-cursor mini menu. */
void battle_state_handle_mini_menu_help_state(void) {
    if (battle_menu_get_thread_activity() != 2) {
        g_battle_menu_help_opening = 0;
        battle_state_enter_mini_menu();
    }
    battle_state_update_units();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
}
