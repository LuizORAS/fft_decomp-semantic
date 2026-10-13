#include "fft/world.h"
#include "psx/types.h"

/* WORLD's version of battle_menu_open_option_at_list: it blocks the help and requests OPTION.OUT,
 * then runs WORLD's resident copy of the AT list (world_option_run_menu). */
void world_menu_open_option_at_list(void) {
    world_menu_block_help();
    world_menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_OPTION);
    world_option_run_menu(OPTION_MENU_AT_LIST);
}
