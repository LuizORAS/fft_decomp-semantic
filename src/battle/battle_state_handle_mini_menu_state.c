#include "fft/battle.h"
#include "psx/pad.h"

/* MINI_MENU: Select opens its help and closing it returns to FREE_CURSOR; an answer of 0x64 or
 * more passes the entry (answer - 0x64) to battle_turn_show_at_entry. */
void battle_state_handle_mini_menu_state(void) {
    s32 menu_result;

    menu_result = battle_menu_get_thread_activity();
    if (g_main_input_pressed_buttons & PSX_PAD_SELECT) {
        battle_state_enter_mini_menu_help();
    } else if (menu_result == 0) {
        g_battle_cursor_input_mode = g_main_game_options.fields.cursor_movement;
        battle_state_enter_free_cursor();
    } else if (menu_result >= 0x64) {
        battle_turn_show_at_entry(menu_result - 0x64);
    }
    battle_state_update_units();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
}
