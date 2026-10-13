#include "fft/battle_menu.h"

/* Reset the selected row of the 11 menu entries listed in
 * g_battle_menu_idle_action_flag_clear_ids. */
void battle_menu_clear_entry_selections(void) {
    s32 i;

    for (i = 0; i < 11; i++) {
        g_battle_menu_thread_menu_data[g_battle_menu_idle_action_flag_clear_ids[i]].selected_index = 0;
    }
}
