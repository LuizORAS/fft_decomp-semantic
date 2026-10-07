#include "fft/main.h"

/* Copy the 24 ability slots of a skillset (skillset 0 for an id of 0x100 or more) into
 * g_main_ability_temp_list, keeping the classes that filters selects (skillset_ability_filter_e) and
 * zeroing the others; returns the list. The support test takes ids below 0x1e7, so Move+1 counts as a
 * support ability (QUIRKS.md); every caller asks for the action abilities or for all of them. */
u16* main_ability_store_skillset_abilities(s32 skillset_id, s32 filters) {
    s32 output_count;
    s32 ability_index;
    s32 ability_id;
    s32 keep;

    if (skillset_id >= 0x100) {
        skillset_id = 0;
    }
    output_count = 0;
    ability_index = 0;
    do {
        ability_id = main_ability_get_id_from_skillset(skillset_id, ability_index);
        if ((u32)(ability_id & 0xFFFF) < ABILITY_ID_REACTION_FIRST) {
            keep = filters & SKILLSET_ABILITY_FILTER_ACTION;
        } else if ((u32)(ability_id & 0xFFFF) < ABILITY_ID_SUPPORT_FIRST) {
            keep = filters & SKILLSET_ABILITY_FILTER_REACTION;
        } else if ((u32)(ability_id & 0xFFFF) < ABILITY_ID_MOVEMENT_FIRST + 1) {
            /* One past ABILITY_ID_MOVEMENT_FIRST in the original. */
            keep = filters & SKILLSET_ABILITY_FILTER_SUPPORT;
        } else {
            keep = filters & SKILLSET_ABILITY_FILTER_MOVEMENT;
        }
        if (keep == 0) {
            ability_id = 0;
        }
        g_main_ability_temp_list[output_count++] = ability_id;
        ability_index += 1;
    } while (ability_index < SKILLSET_ABILITY_LIST_COUNT);

    while (output_count < SKILLSET_ABILITY_LIST_COUNT) {
        g_main_ability_temp_list[output_count] = 0;
        output_count += 1;
    }
    return g_main_ability_temp_list;
}
