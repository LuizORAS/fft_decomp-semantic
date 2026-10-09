#include "fft/battle.h"

void battle_state_handle_unit_move_state(void) {
    s32 selected_ability;

    battle_state_handle_free_cursor_input();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
    selected_ability = *battle_menu_get_selected_ability_address();
    switch (selected_ability) {
    case 7:
        battle_menu_close_move_help();
        return;
    case 8:
    case 0xff:
        battle_menu_open_active_unit_idle_action_menu();
        return;
    }
}
