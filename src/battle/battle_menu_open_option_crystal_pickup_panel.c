#include "fft/battle.h"
#include "fft/event_option.h"
#include "psx/types.h"

/* Thread started for system function 0x1f (crystal or treasure): load OPTION.OUT
 * and open its dead-unit panel. */
void battle_menu_open_option_crystal_pickup_panel(void) {
    battle_menu_block_help();
    battle_menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_OPTION);
    option_entrypoint(OPTION_MENU_CRYSTAL_PICKUP);
}
