/* SCUS_942.21 0x80014278..0x80014357. */
#include "psx/suzuki.h"

void* main_sound_alloc(u32 size) {
    suzuki_heap_block_t* block = g_main_sound_heap_blocks;
    suzuki_heap_block_t* allocated;
    u8* payload;
    /* The unpinned final heap limit swaps the original v1/v0 loads. */
    register u8* heap_end __asm__("$3");
    /* The unpinned final gap remains in v1 instead of the retail v0. */
    register u32 final_gap __asm__("$2");
    u32 required = ((size + 15) & ~15) + sizeof(suzuki_heap_block_t);
    while (block->next != 0) {
        if ((u32)((u8*)block->next - block->end) >= required) {
            goto found;
        }
        block = block->next;
    }
    heap_end = g_main_sound_heap_end;
    final_gap = heap_end - block->end;
    if (final_gap >= required) {
    found:
        allocated = (suzuki_heap_block_t*)(((u32)block->end + 15) & ~15);
        payload = (u8*)(allocated + 1);
        allocated->end = payload + size;
        allocated->next = 0;
        allocated->_unknown_04 = 0;
        allocated->flags = 2;
        allocated->_unknown_02 = 0;
        allocated->next = block->next;
        block->next = allocated;
        main_sound_clear_memory(payload, size);
        return payload;
    }
    return 0;
}
