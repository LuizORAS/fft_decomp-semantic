#include "fft/battle.h"

/* Return the position of the unit's first entry on a freshly built AT list without charged
 * actions (mode 1): -1 for an empty slot, -3 for a unit that cannot act, -2 when it is not
 * listed. */
s32 battle_turn_find_unit_in_at_list(battle_stats_t* unit) {
    battle_at_entry_t at_list[40];
    s32 i;
    battle_at_entry_t* entry;
    u8 misc_unit_id;

    if (unit->entd_slot == BATTLE_ENTD_SLOT_NONE)
        return -1;
    if (battle_status_is_unit_absent_dead_crystal_treasure_petrified_or_ridden(unit) != 0)
        return -3;
    misc_unit_id = unit->misc_unit_id;
    battle_turn_build_at_list(at_list, 1);
    i = 0;
    entry = at_list;
    do {
        if ((entry->unit & 0x1F) == (misc_unit_id & 0xFF)) {
            return i;
        }
        i += 1;
        entry++;
    } while (i < 0x28);
    return -2;
}
