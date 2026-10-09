#include "fft/event_bunit.h"
#include "psx/types.h"

s32 bunit_ability_get_ct_display_value(s32 index) {
    s32 entry = g_bunit_ability_entries[index];
    s32 result;

    if (entry & ABILITY_LIST_ENTRY_HIDE_DETAILS) {
        result = MENU_NUMBER_VALUE_DASHES;
    } else {
        result = ((ability_secondary_data_t*)g_bunit_ability_selected_specific)->ct;
        result = (100 / result) + ((100 % result) != 0);
    }
    if (entry >> ABILITY_LIST_ENTRY_DISABLED_SHIFT) {
        result |= MENU_NUMBER_VALUE_DISABLED;
    }
    return result;
}
