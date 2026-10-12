#include "fft/battle.h"
#include "fft/battle_action.h"

/* Open the status menu once: mark it open and restart the AT list cursor at entry
 * 0. */
void battle_action_open_status_menu_at_list_start(void) {
    if (g_battle_status_menu_open == 0) {
        g_battle_status_menu_open = 1;
        g_battle_action_at_list_id = 0;
    }
}
