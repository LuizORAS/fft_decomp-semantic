#include "fft/world.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_get_selected_command_address. */
s32* world_menu_get_selected_command_address(void) {
    return &g_world_menu_selected_command;
}
