#include "fft/battle.h"
#include "fft/battle_target.h"

/* ILLEGAL_RANGE: any answer to the out-of-range message returns to target selection. */
void battle_state_handle_illegal_range_state(void) {
    s32 command;

    battle_state_update_units();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
    command = battle_menu_get_selected_command_address()->kind;
    if (command >= 7 && (command < 9 || command == 0xff)) {
        battle_target_begin_tile_selection();
    }
}
