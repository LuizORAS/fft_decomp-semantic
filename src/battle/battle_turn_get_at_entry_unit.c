#include "fft/battle_action.h"
#include "psx/types.h"

/* Rebuild the AT list and return the unit id of the chosen entry.
 *
 * Returns -4 for an empty entry; a charged-action entry (bit 0x40) returns its
 * unit id plus 0x100. */
s32 battle_turn_get_at_entry_unit(s32 at_index) {
    battle_at_entry_t at_list[BATTLE_AT_LIST_CAPACITY];
    u32 entry;
    u32 unit_id;

    battle_turn_build_at_list(at_list, BATTLE_AT_LIST_MODE_ALL);
    entry = at_list[at_index].unit_code;
    unit_id = entry & WORLD_AT_UNIT_ID_MASK;
    if (unit_id == WORLD_AT_END_OF_LIST) {
        return -4;
    }
    if (entry & WORLD_AT_ACTING_UNIT) {
        unit_id += 0x100;
    }
    return unit_id;
}
