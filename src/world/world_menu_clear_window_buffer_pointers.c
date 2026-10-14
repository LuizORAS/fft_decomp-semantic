#include "fft/menu.h"
#include "fft/world.h"

/* Mark all six window buffer pointers (three slots, two frame buffers each)
 * unused with -1. */
void world_menu_clear_window_buffer_pointers(void) {
    menu_window_buffer_t* empty = (menu_window_buffer_t*)-1;
    s32 index;

    for (index = 5; index >= 0; index--) {
        g_menu_window_buffer_slots[index] = empty;
    }
}
