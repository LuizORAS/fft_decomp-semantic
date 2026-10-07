#include "fft/battle.h"

/* AFTER_COMMAND: once the system function has answered and the source unit's number display
 * has ended, enter CONTINUE_TURN. */
void battle_state_handle_after_command_state(void) {
    s32* command_address;
    s32 command;
    battle_unit_misc_data_t* unit;

    battle_state_update_units();
    battle_menu_draw_selection_data(main_gfx_get_otag(), g_controller_input_raw);
    command_address = battle_menu_get_selected_command_address();
    unit = battle_unit_get_source_misc_data();
    command = *command_address;
    if ((command >= 7) && ((command < 9) || (command == 0xFF))) {
        g_battle_action_post_action = 1;
    }
    if ((g_battle_action_post_action != 0) && (unit->numeric_display_active == 0)) {
        battle_state_enter_continue_turn();
    }
}
