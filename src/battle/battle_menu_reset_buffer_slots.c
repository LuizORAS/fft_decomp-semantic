#include "fft/battle_menu.h"
#include "psx/types.h"

/* Mark all three menu-buffer pointer pairs as unallocated. */
void battle_menu_reset_buffer_slots(void) {
    menu_window_buffer_t* value = (menu_window_buffer_t*)-1;
    s32 index;

    for (index = 5; index >= 0; index--) {
        g_battle_menu_buffer_slots[index] = value;
    }
}
