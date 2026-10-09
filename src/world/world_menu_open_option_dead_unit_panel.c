#include "fft/world.h"
#include "psx/types.h"

void world_menu_open_option_dead_unit_panel(void) {
    world_menu_set_transition_active_flag();
    world_menu_request_open_companion_executable(1);
    world_menu_run_main_mode(2);
}
