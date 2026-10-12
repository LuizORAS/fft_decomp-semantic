#include "fft/battle_effect.h"

/* Unlinks slot id from the active chain and pushes it onto the free chain;
 * slot 0 is the null sentinel and is ignored. */
void effect_free_slot(u8 id) {
    u8 free_head;

    if (id == 0) {
        return;
    }
    g_effect_slots[g_effect_slot_free_head].prev = id;
    if (g_effect_slots[id].prev != 0) {
        g_effect_slots[g_effect_slots[id].prev].next = g_effect_slots[id].next;
    } else {
        g_effect_active_slot_head = g_effect_slots[id].next;
    }
    g_effect_slots[g_effect_slots[id].next].prev = g_effect_slots[id].prev;
    free_head = g_effect_slot_free_head;
    g_effect_slots[id].prev = 0;
    g_effect_slots[id].next = free_head;
    g_effect_slot_free_head = id;
}
