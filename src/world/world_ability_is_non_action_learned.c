#include "fft/world.h"
#include "psx/types.h"

/* Caches the ability record pointers and type of one ability list entry and
 * returns 1 for an ability of any type other than ABILITY_TYPE_DEFAULT whose
 * entry bits 14-15 are clear; 0 for an ABILITY_TYPE_DEFAULT ability (one
 * with a range record) or a disabled entry. */
s32 world_ability_is_non_action_learned(s32 index) {
    s32 result;

    g_world_ability_selected_type
        = main_ability_calculate_pointers_and_type(g_world_ability_entries[index] & ABILITY_LIST_ENTRY_ID_MASK,
            &g_world_selected_ability_data, &g_world_selected_ability_secondary_data);
    result = 0;
    if (g_world_ability_selected_type != 0) {
        result = (g_world_ability_entries[index] >> ABILITY_LIST_ENTRY_DISABLED_SHIFT) == 0;
    }
    return result;
}
