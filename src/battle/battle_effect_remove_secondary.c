#include "fft/battle.h"

/* Take secondary effect record id (1-16) off the active chain, free its work
 * block and append it to the end of the free chain headed by
 * g_battle_effect_free_slot_head: the inverse of
 * battle_effect_allocate_secondary_slot. An id outside 1-16 only reaches the
 * empty report hook with code 0x67.
 *
 * Both chains are walked through previous_slot_id. A record removed from the
 * middle of the active chain passes its previous link to the record after it, but
 * the record before it keeps next_slot_id pointing at the freed record; only the
 * tail's next link is cleared.
 *
 * The free-chain walk index is an s32: a u16 adds an `andi 0xffff` per load. */
void battle_effect_remove_secondary(u8 id) {
    battle_effect_secondary_data_t* slot;
    s32 tail;

    slot = &g_battle_effect_secondary_data[id];
    if (id != 0 && id < 17) {
        if (slot->allocation != 0) {
            main_heap_free(slot->allocation);
            slot->allocation = 0;
        }
        if (slot->next_slot_id != 0) {
            g_battle_effect_secondary_data[slot->next_slot_id].previous_slot_id = slot->previous_slot_id;
        } else {
            g_battle_effect_active_slot_tail = slot->previous_slot_id;
        }
        g_battle_effect_secondary_data[g_battle_effect_active_slot_tail].next_slot_id = 0;
        tail = g_battle_effect_free_slot_head;
        if (tail != 0) {
            while (g_battle_effect_secondary_data[tail].previous_slot_id != 0) {
                tail = g_battle_effect_secondary_data[tail].previous_slot_id;
            }
            g_battle_effect_secondary_data[tail].previous_slot_id = id;
            slot->next_slot_id = tail;
        } else {
            g_battle_effect_free_slot_head = id;
            slot->next_slot_id = 0;
        }
        slot->previous_slot_id = 0;
        slot->phase = BATTLE_SECONDARY_EFFECT_ENDED;
        return;
    }
    main_system_report_error_2(1, 0x67);
}
