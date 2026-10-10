#include "fft/battle.h"

/* Allocate `size` bytes from the battle heap for owner list `kind` (0-15) and
 * return the address past the block header.
 *
 * First fit along the circular free list, starting after the rover as in the K&R
 * malloc: the request is rounded up to 8-byte units plus one unit for the header,
 * an exact fit is unlinked and a larger block gives up its tail. The block goes
 * to the head of the owner's list. There is no failure exit: when no free block
 * is large enough the search goes round the list without end. */
void* battle_heap_alloc_block(u32 size, s32 kind) {
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
            node->owner = kind;
            node->next = g_battle_heap_owner_lists[kind].head;
            g_battle_heap_rover = prev;
            g_battle_heap_owner_lists[kind].head = node;
            return node + 1;
        } else {
            prev = node;
            node = node->next;
        }
    }
}
