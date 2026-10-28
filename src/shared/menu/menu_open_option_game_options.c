#include "fft/battle_menu.h"
#include "fft/event_option.h"
#include "fft/menu.h"

/* Thread entry of menu entry 0x0b: load OPTION.OUT and open the game options. WORLD's copy blocks the
 * help, requests OPTION.OUT and runs WORLD's resident copy of OPTION's code (world_option_run_menu). */
void menu_open_option_game_options(void) {
    battle_menu_set_modal_open();
    menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_OPTION);
    option_entrypoint(OPTION_MENU_GAME_OPTIONS);
}
