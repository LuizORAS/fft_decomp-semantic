#include "fft/battle_effect.h"

void effect_free_list_node(effect_list_node_t* node) {
    u16* free_head;

    if (node->sprite_block != 0) {
        effect_free_sprite_block(node->sprite_block);
        node->sprite_block = 0;
    }
    if (node->prev != 0) {
        g_effect_list_nodes[node->prev].next = node->next;
    } else {
        g_effect_active_list_node_head = node->next;
    }
    g_effect_list_nodes[node->next].prev = node->prev;
    free_head = &g_effect_free_list_node_head;
    node->next = *free_head;
    node->prev = 0;
    *free_head = node->index;
}
