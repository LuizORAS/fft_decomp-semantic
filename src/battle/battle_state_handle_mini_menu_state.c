#include "fft/battle.h"
#include "psx/pad.h"

/* MINI_MENU: Select opens its help and closing it returns to FREE_CURSOR; an answer of 0x64 or
 * more passes the entry (answer - 0x64) to battle_action_get_next_acting_unit. */
void battle_state_handle_mini_menu_state(void) {
    s32 menu_result;

    menu_result = battle_menu_is_still_building();
    if (g_controller_input_pressed & PSX_PAD_SELECT) {
        battle_menu_open_mini_menu_help();
    } else if (menu_result == 0) {
        g_battle_controller_input = g_main_game_options.fields.cursor_movement;
        battle_state_enter_free_cursor();
    } else if (menu_result >= 0x64) {
        battle_action_get_next_acting_unit(menu_result - 0x64);
    }
    battle_state_update_units();
    battle_menu_draw_selection_data(main_gfx_get_otag(), g_controller_input_raw);
}
