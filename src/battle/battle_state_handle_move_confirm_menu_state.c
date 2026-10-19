#include "fft/battle.h"
#include "psx/types.h"

/* MOVE_CONFIRM_MENU: after the walk, 7 keeps the move (the rider's data goes to the mount); 8 or
 * cancel undoes it and returns to choosing the destination, except after a teleport, which is
 * kept. */
void battle_state_handle_move_confirm_menu_state(void) {
    battle_ai_command_t* command_address;
    s32 command;
    battle_unit_t* source_unit;
    battle_unit_t* casting_unit;

    battle_state_update_units();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
    command_address = battle_menu_get_selected_command_address();
    source_unit = battle_unit_get_source();
    casting_unit = battle_unit_get_casting();
    command = command_address->kind;
    switch (command) {
    case 8:
    case 0xff:
        if (casting_unit->path.flags & BATTLE_EFFECTIVE_MOVEMENT_TELEPORT) {
        case 7:
            battle_unit_copy_rider_data_to_mount(source_unit, casting_unit);
            return;
        }
        battle_move_undo_unit_move(casting_unit);
        battle_gfx_update_unit_palettes();
        battle_state_enter_select_move_tile();
        return;
    }
}
