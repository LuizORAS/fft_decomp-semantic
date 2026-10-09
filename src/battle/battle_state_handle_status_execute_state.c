#include "fft/battle.h"
#include "psx/types.h"

void battle_state_handle_status_execute_state(void) {
    s32 id;
    battle_unit_misc_data_t* misc;

    battle_state_handle_free_cursor_input();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
    id = *battle_menu_get_selected_command_address();
    if ((id >= 7) && ((id < 9) || (id == 0xFF))) {
        g_battle_action_post_action = 1;
    }
    if (g_battle_action_post_action != 0) {
        if (g_battle_state_animation_continue_check == 0) {
            misc = battle_unit_get_source_misc_data();
            if (misc->numeric_display_active == 0) {
                battle_unit_update_display_by_misc_id(misc->unit_id);
                battle_action_check_between_turn_events();
            }
        }
    }
}
