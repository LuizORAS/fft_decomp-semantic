#include "fft/main.h"
#include "psx/types.h"

/* Same as main_heap_free, for the SMD heap. */
s32 main_heap_free_smd(void* allocation) {
    u32 index;
    s32 tag;
    u8* entry;

    index = ((u32)allocation - (u32)g_main_heap_smd_base) >> 11;
    tag = g_main_heap_smd_allocator_table[index];
    if (((index == 0) | (tag != g_main_heap_smd_allocator_table[index - 1])) == 0) {
        return 0;
    }
    entry = &g_main_heap_smd_allocator_table[index];
    do {
        *entry = 0;
        entry += 1;
    } while (*entry == tag);
    return 1;
}
