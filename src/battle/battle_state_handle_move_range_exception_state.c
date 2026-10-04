#include "fft/battle.h"

/* MOVE_RANGE_EXCEPTION: a message window; any answer opens the idle action menu. */
void battle_state_handle_move_range_exception_state(void) {
    s32 command;

    battle_state_update_units();
    battle_menu_draw_selection_data(main_gfx_get_otag(), g_controller_input_raw);
    command = *battle_menu_get_selected_command_address();
    if (command >= 7 && (command < 9 || command == 0xff)) {
        battle_menu_dispatch_idle_action_menu();
    }
}
