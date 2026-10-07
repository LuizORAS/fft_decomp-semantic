#include "fft/battle.h"

/* ILLEGAL_MOVE_MENU: the unreachable-tile message; any answer returns to choosing the destination. */
void battle_state_handle_illegal_move_menu_state(void) {
    s32 command;

    battle_state_update_units();
    battle_menu_draw_selection_data(main_gfx_get_otag(), g_controller_input_raw);
    command = *battle_menu_get_selected_command_address();
    if (command >= 7 && (command < 9 || command == 0xff)) {
        battle_menu_close_move_help();
    }
}
