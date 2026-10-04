#include "fft/battle.h"

/* FREE_CURSOR_HELP: once the help window has closed, return to FREE_CURSOR. */
void battle_state_handle_free_cursor_help_state(void) {
    if (battle_menu_is_still_building() != 2) {
        g_battle_menu_help_opening = 0;
        battle_state_enter_free_cursor();
    }
    battle_state_update_units();
    battle_menu_draw_selection_data(main_gfx_get_otag(), g_controller_input_raw);
}
