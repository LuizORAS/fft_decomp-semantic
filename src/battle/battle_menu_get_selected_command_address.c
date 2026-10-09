#include "fft/battle.h"
#include "psx/types.h"

s32* battle_menu_get_selected_command_address(void) {
    return &g_battle_menu_selected_command;
}
