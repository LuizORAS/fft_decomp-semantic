#include "fft/battle.h"

/* Reset the selected row of the 11 menu entries listed in
 * g_battle_menu_cleared_entry_indices. */
void battle_menu_clear_entry_selections(void) {
    s32 i;
    world_menu_entry_t* entries;

    i = 0;
    entries = g_battle_menu_thread_menu_data;
    do {
        entries[g_battle_menu_cleared_entry_indices[i]].selected_index = 0;
        i += 1;
    } while (i < 11);
}
