#include "fft/world.h"
#include "psx/types.h"

/* Opens menu entry `index` on thread 8, first placing its window at the default
 * origin and clamping that origin so the window stays on screen. */
void world_menu_open_entry_window(s32 index, s32 unused_x, s32 unused_y) {
    world_menu_entry_t* entry;
    s32 x;
    s32 y;
    s32 width;
    s32 height;
    s32 initial_x = -0x70;
    s32 initial_y = -0x50;

    if (g_world_threads[NATIVE_THREAD_SLOT_MENU].is_running == 0) {
        entry = &g_world_menu_default_entries[index];
        g_world_menu_thread_menu_data = g_world_menu_default_entries;
        entry->window_x = initial_x;
        entry->window_y = initial_y;
        x = entry->window_x;
        g_world_menu_open_entry_index = index;
        width = entry->window_width;
        if (x + width >= 0x81) {
            entry->window_x = 0x78 - entry->window_width;
        } else if (x < -0x80) {
            entry->window_x = -0x78;
        }
        height = entry->window_height;
        y = entry->window_y;
        if (y + height >= 0x79) {
            entry->window_y = 0x70 - entry->window_height;
        } else if (y < -0x78) {
            entry->window_y = -0x70;
        }
        world_thread_start(NATIVE_THREAD_SLOT_MENU, g_world_menu_thread_menu_data[index].thread_entry);
        world_thread_set_parameters(NATIVE_THREAD_SLOT_MENU, (s32)&g_world_menu_thread_menu_data[index], 0, 0);
    }
}
