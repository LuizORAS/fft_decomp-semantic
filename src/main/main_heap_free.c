#include "fft/main.h"
#include "psx/types.h"

/* Free the game-heap block that starts at pointer: clear its page tags from that
 * page on while they repeat the block's tag. Returns 1, or 0 without freeing when
 * the page below holds the same tag (pointer is taken to be inside a block).
 *
 * A block directly above with the same tag (see main_heap_alloc) is freed with
 * it. For page 0 the test also reads the byte before the table. */
s32 main_heap_free(void* pointer) {
    u32 page_index = (u32)((u8*)pointer - g_main_heap_high_overlay_load_address) >> MAIN_HEAP_PAGE_SHIFT;
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
