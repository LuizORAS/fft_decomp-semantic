#include "fft/main.h"
#include "psx/types.h"

/* Allocate size bytes from the game heap: 64 pages of 0x800 bytes starting at
 * g_main_heap_high_overlay_load_address, one tag byte per page in
 * g_main_heap_game_allocator_table (0 = free).
 *
 * First fit over whole pages. The new block's tag is one more than the highest
 * tag met before the block, so it differs from the block below it, but the block
 * above can hold the same tag; main_heap_free tells blocks apart by tag alone.
 * Returns 0 when no run of free pages is long enough. */
void* main_heap_alloc(u32 size) {
    u32 run = 0;
    u32 needed = size >> 11;
    u32 tag = 1;
    u32 index;
    u32 start;

    if ((size & 0x7ff) != 0) {
        needed++;
    }
    for (index = 0; index < 0x40; index++) {
        u32 cell = g_main_heap_game_allocator_table[index];

        if (cell == 0) {
            if (run == 0) {
                start = index;
            }
            run++;
            if (run >= needed) {
                break;
            }
        } else {
            if (cell >= tag) {
                tag = cell + 1;
            }
            run = 0;
        }
    }
    if (run >= needed) {
        for (index = 0; index < run; index++) {
            g_main_heap_game_allocator_table[start + index] = tag;
        }
        return (start << 11) + g_main_heap_high_overlay_load_address;
    }
    return 0;
}
