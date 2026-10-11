#include "fft/main.h"
#include "psx/types.h"

/* Allocate size bytes from the music heap: 16 pages of 0x800 bytes at
 * g_main_heap_smd_base, tagged in g_main_heap_smd_allocator_table. The algorithm
 * is main_heap_alloc's; returns 0 when no run of free pages is long enough. */
void* main_heap_alloc_smd(u32 size) {
    u32 run = 0;
    u32 needed = size >> MAIN_HEAP_PAGE_SHIFT;
    u32 tag = 1;
    u32 index;
    u32 start;

    if ((size & (MAIN_HEAP_PAGE_SIZE - 1)) != 0) {
        needed++;
    }
    index = 0;
    do {
        u32 cell;

        cell = g_main_heap_smd_allocator_table[index];
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
        index++;
    } while (index < MAIN_HEAP_SMD_PAGE_COUNT);
    if (run >= needed) {
        for (index = 0; index < run; index++) {
            g_main_heap_smd_allocator_table[start + index] = tag;
        }
        return &g_main_heap_smd_base[start << MAIN_HEAP_PAGE_SHIFT];
    }
    return 0;
}
