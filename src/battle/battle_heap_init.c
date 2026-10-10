#include "fft/battle.h"

/* Turns buffer into a single free block that links to itself and empties the
 * per-owner lists; the effect stage calls this with the effect palette
 * buffer. */
void battle_heap_init(battle_heap_node_t* buffer, u32 buffer_size) {
    s32 owner;
    s16 block_count;

    block_count = (buffer_size >> 3) - 1;
    g_battle_heap_rover = buffer;
    g_battle_heap_base = buffer;
    buffer->size = block_count;
    g_battle_heap_block_count = block_count;
    buffer->next = buffer;
    for (owner = 16; owner >= 0; owner--) {
        g_battle_heap_owner_lists[owner].head = 0;
    }
}
