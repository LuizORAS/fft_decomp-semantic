#include "fft/event_jobstts.h"
#include "psx/types.h"

s32 jobstts_ability_get_jp_cost_display_value(s32 index) {
    s32 entry = g_jobstts_ability_entries[index];
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
