#include "fft/battle.h"

/* UNIT_MOVE: the Move command's window: 7 goes on to choosing the destination
 * (battle_state_enter_select_move_tile), 8 or cancel returns to the action menu. */
void battle_state_handle_unit_move_state(void) {
    s32 command;

    battle_state_update_units();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
    command = *battle_menu_get_selected_command_address();
    switch (command) {
    case 7:
        battle_state_enter_select_move_tile();
        return;
    case 8:
    case 0xff:
        battle_menu_open_active_unit_idle_action_menu();
        return;
    }
}
