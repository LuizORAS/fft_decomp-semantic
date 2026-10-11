#include "fft/battle.h"
#include "psx/types.h"

/* UnitMovingSetup game-state handler: command 7 starts the move; 8 or 0xff
 * discards the path and returns to choosing a destination. */
void battle_state_handle_unit_moving_setup_state(void) {
    s32* selected_command_address;
    s32 selected_command;
    battle_unit_misc_data_t* casting_misc_data;

    battle_state_handle_free_cursor_input();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
    selected_command_address = battle_menu_get_selected_command_address();
    casting_misc_data = battle_unit_get_casting_misc_data();
    selected_command = *selected_command_address;
    switch (selected_command) {
    case 7:
        battle_state_enter_unit_moving();
        return;
    case 8:
    case 0xff:
        casting_misc_data->path.count = 0;
        battle_menu_close_move_help();
        return;
    }
}
