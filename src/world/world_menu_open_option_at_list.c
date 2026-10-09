#include "fft/world.h"
#include "psx/types.h"

void world_menu_open_option_at_list(void) {
    world_menu_set_transition_active_flag();
    world_menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_OPTION);
    world_menu_run_main_mode(OPTION_MENU_AT_LIST);
}
