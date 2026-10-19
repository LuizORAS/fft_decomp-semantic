#include "fft/menu.h"
#include "fft/world.h"
#include "psx/types.h"

/* Returns the selected action ability's displayed MP cost, read from its range
 * record (g_world_selected_ability_secondary_data.range->mp_cost, byte 0xd). */
s32 world_ability_get_mp_cost_display_value(s32 index) {
    s32 result;
    s32 entry = g_menu_ability_entries[index];

    if (entry & ABILITY_LIST_ENTRY_HIDE_DETAILS) {
        result = MENU_NUMBER_VALUE_DASHES;
    } else {
        result = g_world_selected_ability_secondary_data.range->mp_cost;
    }
    if (entry >> ABILITY_LIST_ENTRY_DISABLED_SHIFT) {
        result |= MENU_NUMBER_VALUE_DISABLED;
    }
    return result;
}
