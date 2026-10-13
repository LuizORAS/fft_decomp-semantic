#include "fft/world.h"
#include "psx/types.h"

/* Run the game options menu (world_option_run_menu with OPTION_MENU_GAME_OPTIONS). */
void world_option_game_options_thread(void) {
    world_option_run_menu(OPTION_MENU_GAME_OPTIONS);
}
