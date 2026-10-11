#include "fft/event_jobstts.h"
#include "psx/types.h"

/* Returns the selected action ability's displayed MP cost, from the
 * secondary record: dashes for an entry whose details are hidden, with the
 * disabled flag added for a disabled entry. */
s32 jobstts_ability_get_mp_cost_display_value(s32 index) {
    s32 entry = g_jobstts_ability_entries[index];
    s32 result;

    if (entry & ABILITY_LIST_ENTRY_HIDE_DETAILS) {
        result = MENU_NUMBER_VALUE_DASHES;
    } else {
        result = g_jobstts_selected_ability_secondary_data.range->mp_cost;
    }
    if (entry >> ABILITY_LIST_ENTRY_DISABLED_SHIFT) {
        result |= MENU_NUMBER_VALUE_DISABLED;
    }
    return result;
}
