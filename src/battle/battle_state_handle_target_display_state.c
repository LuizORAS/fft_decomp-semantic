#include "fft/battle.h"
#include "psx/pad.h"
#include "psx/types.h"

/* TARGET_DISPLAY: Circle or Cross returns the cursor to the casting unit and reopens its action
 * menus. */
void battle_state_handle_target_display_state(void) {
    s32 flags;
    battle_state_update_units();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
    flags = g_main_input_pressed_buttons;
    if ((flags & PSX_PAD_CIRCLE) || (flags & PSX_PAD_CROSS)) {
        battle_target_move_cursor_to_unit(battle_unit_get_casting_misc_data());
        battle_state_enter_open_action_menus();
    }
}
