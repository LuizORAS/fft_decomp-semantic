#include "fft/battle.h"
#include "fft/event_option.h"
#include "psx/types.h"

void battle_menu_open_option_at_list(void) {
    battle_menu_set_option_menu_open();
    battle_menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_OPTION);
    option_entrypoint(OPTION_MENU_AT_LIST);
}
