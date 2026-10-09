/* SCUS_942.21 0x80014278..0x80014357. */
#include "psx/suzuki.h"

void* main_sound_alloc(u32 size) {
    suzuki_heap_block_t* block = g_main_sound_heap_blocks;
    suzuki_heap_block_t* allocated;
    u8* payload;
    u8* limit;
    u32 gap;
    u32 required = ((size + 15) & ~15) + sizeof(suzuki_heap_block_t);
    while (block->next != 0) {
        limit = (u8*)block->next;
        if ((u32)(limit - block->end) >= required) {
            goto found;
        }
        block = block->next;
    }
    limit = g_main_sound_heap_end;
    if ((u32)(limit - block->end) >= required) {
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
