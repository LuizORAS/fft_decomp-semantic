#include "fft/battle.h"
#include "psx/types.h"

/* UNIT_MOVING_SETUP: the move confirmation: 7 starts the move, 8 or cancel drops the path and
 * returns to choosing the destination. */
void battle_state_handle_unit_moving_setup_state(void) {
    battle_ai_command_t* command_address;
    s32 command;
    battle_unit_t* casting_unit;

    battle_state_update_units();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
    command_address = battle_menu_get_selected_command_address();
    casting_unit = battle_unit_get_casting();
    command = command_address->kind;
    switch (command) {
    case 7:
        battle_state_enter_unit_moving();
        return;
    case 8:
    case 0xff:
        casting_unit->path.count = 0;
        battle_state_enter_select_move_tile();
        return;
    }
}
