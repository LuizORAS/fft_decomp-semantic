#include "fft/battle_action.h"
#include "psx/types.h"

/* Count the AT-list entries that resolve before an action by unit_index
 * charging for ct; returns 0xff when no such position exists within 40. */
u32 action_get_number_of_turns_to_resolve(s32 unit_index, s32 ct, battle_at_entry_t* entries) {
    s32 count;
    if (g_battle_between_turn_resume_state == BATTLE_BETWEEN_TURN_STATE_PICK_UNIT_AFTER_CHARGE && ct != 0) {
        ct += 1;
    }
    ct <<= 8;
    for (count = 0; count < BATTLE_AT_LIST_CAPACITY; count++, entries++) {
        s32 entry_unit = entries->unit_code;
        s32 is_action = entry_unit & BATTLE_AT_ACTION_ENTRY;
        s32 entry_ct;
        entry_unit &= BATTLE_AT_UNIT_ID_MASK;
        if (entry_unit == BATTLE_AT_END_OF_LIST)
            return 0xFF;
        entry_ct = entries->sort_key;
        if (entry_ct >= ct) {
            if (ct != entry_ct)
                return count & 0xFF;
            if (unit_index == entry_unit && is_action != 0)
                return count & 0xFF;
            if (entry_unit >= unit_index)
                return count & 0xFF;
            if (is_action == 0)
                return count & 0xFF;
        }
    }
    return 0xFF;
}
