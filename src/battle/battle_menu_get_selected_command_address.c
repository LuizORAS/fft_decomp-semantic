#include "fft/battle.h"
#include "psx/types.h"

battle_ai_command_t* battle_menu_get_selected_command_address(void) {
    return &g_battle_menu_selected_command;
}
