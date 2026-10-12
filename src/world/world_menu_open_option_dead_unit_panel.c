#include "fft/world.h"
#include "psx/types.h"

/* Menu thread: load the OPTION overlay and run its dead unit panel mode. */
void world_menu_open_option_dead_unit_panel(void) {
    world_menu_block_help();
    world_menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_OPTION);
    world_menu_run_main_mode(OPTION_MENU_DEAD_UNIT);
}
