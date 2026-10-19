#include "fft/battle_action.h"
#include "fft/battle_status.h"

/* Return the position of the unit's first entry on a freshly built AT list without charged
 * actions (mode 1): -1 for an empty slot, -3 for a unit that cannot act, -2 when it is not
 * listed. */
s32 battle_turn_find_unit_in_at_list(const battle_stats_t* unit) {
    battle_at_entry_t at_list[BATTLE_AT_LIST_CAPACITY];
    s32 i;
    battle_at_entry_t* entry;
    u8 battle_id;

    if (unit->entd_slot == BATTLE_ENTD_SLOT_NONE)
        return -1;
    if (battle_status_is_unit_absent_dead_crystal_treasure_petrified_or_ridden(unit) != 0)
        return -3;
    battle_id = unit->battle_id;
    battle_turn_build_at_list(at_list, BATTLE_AT_LIST_MODE_TURNS_ONLY);
    for (i = 0; i < BATTLE_AT_LIST_CAPACITY; i++) {
        entry = &at_list[i];
        if ((entry->unit_code & WORLD_AT_UNIT_ID_MASK) == (battle_id & 0xFF)) {
            return i;
        }
    }
    return -2;
}
