#include "fft/battle.h"
#include "psx/types.h"

/* Insert an entry into the 40-entry AT list before the first one with a larger key, moving the
 * rest down (the last drops off): a turn stores the unit id, an action (mode 1) the id | 0x40,
 * plus 0x20 for a Jump or 0x80 for an ability id above 0xff, with the ability id's low byte.
 * Returns 1 when the entry was placed, 0 when no entry has a larger key. */
s32 battle_turn_insert_at_entry(s32 unit_id, s32 mode, s32 sort_key, battle_at_entry_t* list) {
    battle_stats_t* unit;
    s32 i;
    s32 j;
    s32 unit_code;
    s32 ability_id;
    s32 ability_code;
    s32 cmp;
    u16 key = sort_key;

    unit = &g_battle_unit_stats[unit_id];
    for (i = 0; i < 0x28; i++) {
        cmp = key < list[i].sort_key;
        ability_code = unit_id + 0x40;
        if (cmp) {
            for (j = 0x27; j > i; j--) {
                list[j] = list[j - 1];
            }
            unit_code = unit_id;
            list[i].sort_key = key;
            if (mode != 0) {
                unit_code = ability_code;
                if (unit->last_skillset_id == SKILLSET_ID_JUMP) {
                    unit_code = unit_id + 0x60;
                } else {
                    ability_id = unit->last_ability_id;
                    list[i].flags = ability_id;
                    if (ability_id >= 0x100) {
                        unit_code = unit_id - 0x40;
                    }
                }
            }
            list[i].unit = unit_code;
            break;
        }
    }
    return i != 0x28;
}
