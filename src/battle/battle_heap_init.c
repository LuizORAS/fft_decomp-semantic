#include "fft/battle.h"

/* Turn buffer into one free block that links to itself (buffer_size / 8 - 1 units of
 * 8 bytes) and empty the 17 owner lists. The effect stage gives it the space from the effect
 * palette buffer up to g_battle_heap_end_address (0x801df000, MAIN's game heap). */
void battle_heap_init(battle_heap_node_t* buffer, u32 buffer_size) {
    s32 offset;
    s16 block_count;

    block_count = (buffer_size >> 3) - 1;
    g_battle_heap_rover = buffer;
    g_battle_heap_base = buffer;
    buffer->size = block_count;
    g_battle_heap_block_count = block_count;
    buffer->next = buffer;
    /* The target clears the 17 list heads by walking a byte offset downwards. */
    for (offset = 16 * sizeof(battle_heap_owner_list_t); offset >= 0; offset -= sizeof(battle_heap_owner_list_t)) {
        *(battle_heap_node_t**)((u8*)g_battle_heap_owner_lists + offset) = 0;
    }
}
