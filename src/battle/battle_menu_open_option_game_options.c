#include "fft/battle.h"
#include "fft/event_option.h"
#include "psx/types.h"

/* Thread entry of menu entry 0x0b: load OPTION.OUT and open the game options. */
void battle_menu_open_option_game_options(void) {
    battle_menu_set_option_menu_open();
    battle_menu_request_open_companion_executable(COMPANION_OVERLAY_OPTION);
    option_entrypoint(OPTION_MENU_GAME_OPTIONS);
}
