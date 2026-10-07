#include "fft/battle.h"
#include "psx/types.h"

/* Open the status menu at the first AT list entry unless it is open. */
void battle_turn_start_at_list_browse(void) {
    if (g_battle_status_menu_open == 0) {
        g_battle_status_menu_open = 1;
        g_battle_turn_at_list_index = 0;
    }
}
