#include "fft/battle.h"
#include "psx/types.h"

/* Mark all three menu-buffer pointer pairs as unallocated. */
void battle_menu_clear_window_buffer_pointers(void) {
    s32 value = -1;
    s32 index = 5;
    s32* slot = (s32*)&g_battle_menu_window_buffer_pointers[5];

    do {
        *slot = value;
        index -= 1;
        slot -= 1;
    } while (index >= 0);
}
