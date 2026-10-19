#include "fft/battle_menu.h"

/* Reset the selected row of the 11 menu entries listed in
 * g_battle_menu_cleared_entry_indices. */
void battle_menu_clear_entry_selections(void) {
    s32 i;

    for (i = 0; i < 11; i++) {
        g_menu_thread_menu_data[g_battle_menu_cleared_entry_indices[i]].selected_index = 0;
    }
}
