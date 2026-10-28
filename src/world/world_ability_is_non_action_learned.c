#include "fft/menu.h"
#include "fft/world.h"
#include "psx/types.h"

/* Cache the record pointers and the class (g_world_ability_selected_type, ability_type_e) of
 * ability list entry index, and return 1 for an ability outside the default class whose entry has no
 * flag in bits 14-15, else 0; menu script callback 4 of the view and learn lists. */
s32 world_ability_is_non_action_learned(s32 index) {
    s32 result;

    g_world_ability_selected_type
        = main_ability_calculate_pointers_and_type(g_menu_ability_entries[index] & ABILITY_LIST_ENTRY_ID_MASK,
            &g_world_selected_ability_data, &g_world_selected_ability_secondary_data);
    result = 0;
    if (g_world_ability_selected_type != 0) {
        result = (g_menu_ability_entries[index] >> ABILITY_LIST_ENTRY_DISABLED_SHIFT) == 0;
    }
    return result;
}
