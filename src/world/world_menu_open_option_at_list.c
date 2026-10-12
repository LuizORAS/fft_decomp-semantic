#include "fft/world.h"
#include "psx/types.h"

/* Menu thread: load the OPTION overlay and run its AT list mode. */
void world_menu_open_option_at_list(void) {
    world_menu_block_help();
    world_menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_OPTION);
    world_menu_run_main_mode(OPTION_MENU_AT_LIST);
}
