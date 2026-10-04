#include "fft/battle.h"
#include "psx/types.h"

/* WAIT_MENU: 7 goes on to choosing a facing; 8 or cancel enters AFTER_COMMAND, unless
 * battle_turn_is_over says the turn must change, when a facing is chosen
 * anyway. */
void battle_state_handle_wait_menu_state(void) {
    s32* command_address;
    s32 command;
    battle_unit_misc_data_t* source_misc_data;

    battle_state_update_units();
    battle_menu_draw_selection_data(main_gfx_get_otag(), g_controller_input_raw);
    command_address = battle_menu_get_selected_command_address();
    source_misc_data = battle_unit_get_source_misc_data();
    command = *command_address;
    switch (command) {
    case 8:
    case 0xff:
        if (battle_turn_is_over(source_misc_data->battle_data->misc_unit_id) != 1) {
            battle_state_enter_after_command();
            return;
        }
    case 7:
        battle_action_choose_facing_for_wait();
        return;
    }
}
