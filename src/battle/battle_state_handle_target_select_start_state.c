#include "fft/battle.h"
#include "psx/types.h"

/* TARGET_SELECT_START: any answer to the prompt enters TARGET_SELECT. */
void battle_state_handle_target_select_start_state(void) {
    s32 command;

    battle_state_update_units();
    battle_menu_draw_selection_data(main_gfx_get_otag(), g_controller_input_raw);
    command = *battle_menu_get_selected_command_address();
    if (command >= 7 && (command < 9 || command == 0xff)) {
        battle_state_enter_target_select();
    }
}
