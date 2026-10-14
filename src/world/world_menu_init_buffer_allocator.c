#include "fft/world.h"
#include "psx/types.h"

/* Reset the menu buffer allocator: mark the 16 allocation records free (-1),
 * clear the page allocation map and zero the three window slots' owner
 * thread ids and states. */
void world_menu_init_buffer_allocator(void) {
    s32 fill;
    s32 slot;
    s32 count;
    u8* byte;
    s32* first;
    s32* second;

    /* One counter local shared by the last two loops and each counter set
     * before its pointer: this reproduces the target's register assignment
     * without pins. */
    fill = -1;
    for (slot = MENU_BUFFER_ALLOCATION_COUNT - 1; slot >= 0; slot--) {
        g_world_menu_buffer_allocations[slot].buffer = (u8*)fill;
    }
    count = WORLD_MENU_BUFFER_PAGE_COUNT - 1;
    byte = &g_world_menu_buffer_allocation_map[WORLD_MENU_BUFFER_PAGE_COUNT - 1];
    for (; count >= 0; count--) {
        *byte = 0;
        byte--;
    }
    count = 0;
    second = g_text_slot_glyph_counts;
    first = g_text_slot_owner_thread_ids;
    for (; count < 3; count++) {
        *first = 0;
        *second = 0;
        second++;
        first++;
    }
}
