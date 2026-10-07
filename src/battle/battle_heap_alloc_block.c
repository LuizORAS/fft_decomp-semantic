#include "fft/battle.h"

/* Allocate size bytes plus an 8-byte header from the battle heap: first fit from the rover,
 * splitting the end off a larger free block, then link the block into owner's list. There is
 * no out-of-memory exit: when no free block fits, it walks the free ring forever. */
void* battle_heap_alloc_block(u32 size, s32 owner) {
    battle_heap_node_t* prev;
    battle_heap_node_t* node;
    u32 blocks;

    blocks = (size + 15) >> 3;
    prev = g_battle_heap_rover;
    node = prev->next;
    for (;;) {
        if (node->size >= blocks) {
            if (node->size == blocks) {
                prev->next = node->next;
            } else {
                node->size = node->size - blocks;
                node = (battle_heap_node_t*)((u8*)node + node->size * 8);
                node->size = blocks;
            }
            node->owner = owner;
            node->next = g_battle_heap_owner_lists[owner].head;
            g_battle_heap_rover = prev;
            g_battle_heap_owner_lists[owner].head = node;
            return node + 1;
        } else {
            prev = node;
            node = node->next;
        }
    }
}
