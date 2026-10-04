#include "fft/battle.h"
#include "psx/types.h"

/* UNIT_MOVING_SETUP: the move confirmation: 7 starts the move, 8 or cancel drops the path and
 * returns to choosing the destination. */
void battle_state_handle_unit_moving_setup_state(void) {
    s32* command_address;
    s32 command;
    battle_unit_misc_data_t* casting_misc_data;

    battle_state_update_units();
    battle_menu_draw_selection_data(main_gfx_get_otag(), g_controller_input_raw);
    command_address = battle_menu_get_selected_command_address();
    casting_misc_data = battle_unit_get_casting_misc_data();
    command = *command_address;
    switch (command) {
    case 7:
        battle_state_enter_unit_moving();
        return;
    case 8:
    case 0xff:
        casting_misc_data->movement_path_count = 0;
        battle_menu_close_move_help();
        return;
    }
}
