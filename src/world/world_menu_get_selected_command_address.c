#include "fft/world.h"
#include "psx/types.h"

s32* world_menu_get_selected_command_address(void) {
    return &g_world_menu_selected_command;
}
