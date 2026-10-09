#include "fft/world.h"
#include "psx/types.h"

s32 world_menu_get_entry_item_count(s32 entry_index) {
    s32 result;

    result = world_item_count_owned_and_equipped(g_world_menu_entry_ids[entry_index] & WORLD_ITEM_ID_MASK);
    if (g_world_menu_entry_ids[entry_index] & WORLD_ITEM_LIST_ENTRY_UNEQUIPPABLE) {
        result |= MENU_NUMBER_VALUE_DISABLED;
    }
    return result;
}
