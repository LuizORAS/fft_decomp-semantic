#include "fft/battle_action.h"
#include "psx/types.h"

/* Insert an entry into the 40-entry AT list before the first one with a larger key, moving the rest
 * down (the last drops off): a turn stores the unit id; an action (mode 1) stores the id | 0x40 with the
 * ability id's low byte, plus 0x80 for an ability id above 0xff, and a Jump the id | 0x60 without an
 * ability byte. Returns 1 when the entry was placed, 0 when no entry has a larger key. */
s32 battle_turn_insert_at_entry(s32 battle_id, s32 mode, s32 sort_key, battle_at_entry_t* list) {
    battle_stats_t* unit;
    s32 i;
    s32 j;
    s32 unit_code;
    s32 ability_id;
    s32 ability_code;
    s32 cmp;
    u16 key = sort_key;

    unit = &g_battle_unit_stats[battle_id];
    for (i = 0; i < BATTLE_AT_LIST_CAPACITY; i++) {
        cmp = key < list[i].sort_key;
        ability_code = battle_id + WORLD_AT_ACTING_UNIT;
        if (cmp) {
            for (j = BATTLE_AT_LIST_CAPACITY - 1; j > i; j--) {
                list[j] = list[j - 1];
            }
            unit_code = battle_id;
            list[i].sort_key = key;
            if (mode != 0) {
                unit_code = ability_code;
                if (unit->action_command.skillset_id == SKILLSET_ID_JUMP) {
                    unit_code = battle_id + (WORLD_AT_ACTING_UNIT | WORLD_AT_SHOW_WAIT_LABEL);
                } else {
                    ability_id = unit->action_command.ability_id;
                    list[i].action_id_low = ability_id;
                    if (ability_id >= 0x100) {
                        unit_code = battle_id - WORLD_AT_ACTING_UNIT;
                    }
                }
            }
            list[i].unit_code = unit_code;
            break;
        }
    }
    return i != BATTLE_AT_LIST_CAPACITY;
}
