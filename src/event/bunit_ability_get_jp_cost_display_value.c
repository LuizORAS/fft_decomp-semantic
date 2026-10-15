#include "fft/event_bunit.h"
#include "psx/types.h"

/* Returns the displayed JP cost of an ability list entry: the ability's JP
 * cost, dashes for an entry whose details are hidden, with the disabled flag
 * added for a disabled entry. */
s32 bunit_ability_get_jp_cost_display_value(s32 index) {
    s32 entry = g_bunit_ability_entries[index];
    s32 result;

    if (entry & ABILITY_LIST_ENTRY_HIDE_DETAILS) {
        result = MENU_NUMBER_VALUE_DASHES;
    } else {
        result = g_main_ability_data[entry & ABILITY_LIST_ENTRY_ID_MASK].jp_cost;
    }
    if (entry >> ABILITY_LIST_ENTRY_DISABLED_SHIFT) {
        result |= MENU_NUMBER_VALUE_DISABLED;
    }
    return result;
}
