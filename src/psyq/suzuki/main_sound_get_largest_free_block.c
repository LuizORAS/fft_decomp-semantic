/* SCUS_942.21 0x800143ac..0x8001442b. */
#include "psx/suzuki.h"

/* Reports the aligned gap, including space required for the next allocation record. */
s32 main_sound_get_largest_free_block(void) {
    suzuki_heap_block_t* block = g_main_sound_heap_blocks;
    s32 largest = 0;
    s32 gap;
    while (block->next != 0) {
        gap = (s32)((u8*)block->next - block->end) & ~15;
        if (gap > largest) {
            largest = gap;
        }
        block = block->next;
    }
    gap = (s32)(g_main_sound_heap_end - block->end) & ~15;
    if (gap > largest) {
        largest = gap;
    }
    return largest;
}
