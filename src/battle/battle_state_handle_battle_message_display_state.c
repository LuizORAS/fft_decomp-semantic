#include "fft/battle.h"
#include "psx/types.h"

/* BATTLE_MESSAGE_DISPLAY: once the message has been answered, queue and show the casting unit's
 * effect messages (battle_action_start_effect_messages). */
void battle_state_handle_battle_message_display_state(void) {
    s32 command;

    battle_state_update_units();
    battle_menu_draw_selection_data(main_gfx_get_otag(), g_controller_input_raw);
    command = *battle_menu_get_selected_command_address();
    if (command >= 7 && (command < 9 || command == 0xff)) {
        g_battle_action_post_action = 1;
    }
    if (g_battle_action_post_action != 0) {
        battle_action_start_effect_messages();
    }
}
