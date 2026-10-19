#include "fft/battle_menu.h"

/* Return the address of g_battle_menu_selected_command. */
battle_ai_command_t* battle_menu_get_selected_command_address(void) {
    return &g_battle_menu_selected_command;
}
