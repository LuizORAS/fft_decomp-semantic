#include "fft/battle_effect.h"

/* Take a secondary effect record from the free chain and append it to the active
 * chain, with its timer and slot ids cleared, and return its id. When no record
 * is free it passes code 0x66 to the empty report hook and returns 0.
 *
 * The id is kept as an s32 byte load and a separate `slot = id & 0xff` indexes
 * the table: the target tests and indexes through the masked copy ($v0) while
 * storing and returning the raw id ($s0), which keeps the error path's `move
 * v0,s0` instead of a folded 0. Writing previous_slot_id before the next_slot_id
 * clear reproduces the target's early g_battle_effect_active_slot_tail load. */
u8 battle_effect_allocate_secondary_slot(void) {
    s32 id;
    s32 slot;
    s32 i;

    id = (u8)g_battle_effect_free_slot_head;
    slot = id & 0xff;
    if (slot != 0) {
        g_battle_effect_free_slot_head = g_battle_effect_secondary_data[slot].previous_slot_id;
        g_battle_effect_secondary_data[g_battle_effect_free_slot_head].next_slot_id = 0;
        g_battle_effect_secondary_data[slot].previous_slot_id = g_battle_effect_active_slot_tail;
        g_battle_effect_secondary_data[slot].next_slot_id = 0;
        g_battle_effect_secondary_data[g_battle_effect_active_slot_tail].next_slot_id = id;
        g_battle_effect_active_slot_tail = id;
        g_battle_effect_secondary_data[slot].timer = 0;
        for (i = 31; i >= 0; i--) {
            g_battle_effect_secondary_data[slot].slot_ids[i] = 0;
        }
        return id;
    }
    main_system_report_stub(1, 0x66);
    return id;
}
