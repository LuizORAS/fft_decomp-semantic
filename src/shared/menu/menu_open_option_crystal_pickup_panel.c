#include "fft/battle_menu.h"
#include "fft/event_option.h"
#include "fft/menu.h"

/* Thread started for system function 0x1f (crystal or treasure): load OPTION.OUT
 * and open its dead-unit panel. WORLD's copy blocks the help, requests OPTION.OUT and runs WORLD's
 * resident copy of the dead-unit panel (world_option_run_menu). */
void menu_open_option_crystal_pickup_panel(void) {
    battle_menu_set_modal_open();
    menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_OPTION);
    option_entrypoint(OPTION_MENU_CRYSTAL_PICKUP);
}
