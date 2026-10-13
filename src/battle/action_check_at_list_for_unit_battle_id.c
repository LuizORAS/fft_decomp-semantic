#include "fft/battle_action.h"
#include "fft/battle_status.h"

/* Return the unit's position on a freshly built AT list.
 *
 * Returns -1 for an empty slot, -3 for a unit that cannot act and -2 when the
 * unit is not on the list. */
s32 action_check_at_list_for_unit_battle_id(battle_stats_t* unit) {
    battle_at_entry_t at_list[40];
    s32 i;
    battle_at_entry_t* entry;
    u8 misc_unit_id;

    if (unit->entd_slot == BATTLE_ENTD_SLOT_NONE)
        return -1;
    if (status_is_unit_absent_dead_crystal_treasure_petrified_or_ridden(unit) != 0)
        return -3;
    misc_unit_id = unit->misc_unit_id;
    action_calculate_at_list(at_list, BATTLE_AT_LIST_MODE_TURNS_ONLY);
    for (i = 0; i < 40; i++) {
        entry = &at_list[i];
        if ((entry->unit & 0x1F) == (misc_unit_id & 0xFF)) {
            return i;
        }
    }
    return -2;
}
