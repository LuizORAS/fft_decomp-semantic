#include "fft/battle.h"
#include "psx/types.h"

/* STATUS_EXECUTE: once the message has been answered, no effect runs and the unit's numbers are
 * gone, refresh its display and run the between-turn events. */
void battle_state_handle_status_execute_state(void) {
    s32 command;
    battle_unit_t* unit;

    battle_state_update_units();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
    command = battle_menu_get_selected_command_address()->kind;
    if ((command >= 7) && ((command < 9) || (command == 0xFF))) {
        g_battle_action_post_action = 1;
    }
    if (g_battle_action_post_action != 0) {
        if (g_battle_state_animation_continue_check == 0) {
            unit = battle_unit_get_source();
            if (unit->numeric_display.active == 0) {
                battle_unit_update_display_by_runtime_id(unit->runtime_id);
                battle_turn_advance();
            }
        }
    }
}
