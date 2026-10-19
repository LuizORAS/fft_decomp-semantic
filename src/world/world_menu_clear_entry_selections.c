#include "fft/world.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_clear_entry_selections. */
void world_menu_clear_entry_selections(void) {
    s32 i = 0;
    world_menu_entry_t* entries = g_menu_thread_menu_data;

    for (; i < 11; i++) {
        entries[g_world_menu_cleared_entry_indices[i]].selected_index = 0;
    }
}
