#include "fft/battle.h"

/* Unlink a sprite node from the active list, release its sprite block and push it on the free list: the
 * inverse of battle_effect_alloc_list_node. battle_effect_free_work_record calls it. */
void battle_effect_free_list_node(effect_list_node_t* node) {
    u16* free_head;

    if (node->sprite_block != 0) {
        battle_effect_free_sprite_block(node->sprite_block);
        node->sprite_block = 0;
    }
    if (node->prev != 0) {
        g_battle_effect_list_nodes[node->prev].next = node->next;
    } else {
        g_battle_effect_active_list_node_head = node->next;
    }
    g_battle_effect_list_nodes[node->next].prev = node->prev;
    free_head = &g_battle_effect_free_list_node_head;
    node->next = *free_head;
    node->prev = 0;
    *free_head = node->index;
}
