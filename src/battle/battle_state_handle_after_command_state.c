#include "fft/battle.h"

void battle_state_handle_after_command_state(void) {
    s32* command_address;
    s32 command;
    battle_unit_misc_data_t* unit;

    battle_state_handle_free_cursor_input();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
    command_address = battle_menu_get_selected_command_address();
    unit = battle_unit_get_source_misc_data();
    command = *command_address;
    if ((command >= 7) && ((command < 9) || (command == 0xFF))) {
        g_battle_action_post_action = 1;
    }
    if ((g_battle_action_post_action != 0) && (unit->numeric_display_active == 0)) {
        battle_state_enter_jp_xp_gain();
    }
}
