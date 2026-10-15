/* SCUS_942.21 0x80014278..0x80014357. */
#include "psx/suzuki.h"

/* Take `size` bytes from the driver's CPU heap and clear them: the first gap after a block (between it and
 * the next, or the heap's end) that holds the size rounded up to 16 plus a block header. Returns the payload,
 * or 0 when no gap is large enough. Blocks stay in address order, so freeing one only widens a gap.
 * SuzukiPutPlaySMD, the snapshots, the SFX record and the waveset use it. */
void* main_sound_alloc(u32 size) {
    suzuki_heap_block_t* block = g_main_sound_heap_blocks;
    suzuki_heap_block_t* allocated;
    u8* payload;
    u8* limit;
    u32 required = ((size + 15) & ~15) + sizeof(suzuki_heap_block_t);
    while (block->next != 0) {
        limit = (u8*)block->next;
        if ((limit - block->end) >= required) {
            goto found;
        }
        block = block->next;
    }
    limit = g_main_sound_heap_end;
    if ((limit - block->end) >= required) {
    found:
        allocated = (suzuki_heap_block_t*)(((u32)block->end + 15) & ~15);
        payload = (u8*)(allocated + 1);
        allocated->end = payload + size;
        allocated->next = 0;
        allocated->_unknown_04 = 0;
        allocated->flags = SUZUKI_HEAP_BLOCK_ALLOCATED;
        allocated->_unknown_02 = 0;
        allocated->next = block->next;
        block->next = allocated;
        main_sound_clear_memory(payload, size);
        return payload;
    }
    return 0;
}
