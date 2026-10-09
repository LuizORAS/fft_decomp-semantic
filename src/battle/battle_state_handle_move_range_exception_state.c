#include "fft/battle.h"

void battle_state_handle_move_range_exception_state(void) {
    s32 selected_ability;

    battle_state_handle_free_cursor_input();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
    selected_ability = *battle_menu_get_selected_ability_address();
    if (selected_ability >= 7 && (selected_ability < 9 || selected_ability == 0xff)) {
        battle_menu_dispatch_idle_action_menu();
    }
}
