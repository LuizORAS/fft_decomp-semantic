#include "fft/main.h"
#include "psx/types.h"

/* Free the game-heap allocation that starts at allocation by clearing its cells' tags, from that
 * cell on while they repeat the tag, so a block directly above with the same tag goes too.
 * Returns 1, or 0 without freeing when the cell before carries the same tag (allocation is not
 * the start of a block); for cell 0 that test reads the byte before the table. */
s32 main_heap_free(void* allocation) {
    u32 page_index = (u32)((u8*)allocation - g_main_heap_high_overlay_load_address) >> MAIN_HEAP_PAGE_SHIFT;
    s32 allocation_id = g_main_heap_game_allocator_table[page_index];
    s32 previous_id = g_main_heap_game_allocator_table[page_index - 1];

    if ((page_index < 1) | (allocation_id != previous_id)) {
        u8* entry = &g_main_heap_game_allocator_table[page_index];

        do {
            *entry++ = 0;
        } while (*entry == allocation_id);
        return 1;
    }
    return 0;
}
