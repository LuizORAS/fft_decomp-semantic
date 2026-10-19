#include "fft/main.h"
#include "fft/menu.h"
#include "fft/menu_script.h"
#include "psx/types.h"

/* JOBSTTS binds both output slots to one address; the secondary pointer overwrites the ability pointer. */

/* Caches the ability record pointers and type of one ability list entry and
 * returns 1 for an ability of any type other than ABILITY_TYPE_DEFAULT whose
 * entry bits 14-15 are clear; 0 for an ABILITY_TYPE_DEFAULT ability (one
 * with a range record) or a disabled entry. BUNIT's menu_ability_is_non_action_learned is another
 * copy.
 *
 * JOBSTTS's caller passes one variable for both outputs, under two names so that a1 and a2 are
 * materialized separately: the ability data pointer is overwritten by the secondary data pointer. */
s32 menu_ability_is_non_action_learned(s32 index) {
    s16* entries;
    s16* entry;
    s32 result;
    u16 ability;

    entries = g_menu_ability_entries;
    entry = &entries[index];
    g_menu_ability_type = main_ability_calculate_pointers_and_type(
        *entry & ABILITY_LIST_ENTRY_ID_MASK, &g_menu_selected_ability_data, &g_menu_selected_ability_secondary_data);
    result = 0;
    if (g_menu_ability_type != ABILITY_TYPE_DEFAULT) {
        ability = *entry;
        result = (ability >> ABILITY_LIST_ENTRY_DISABLED_SHIFT) == 0;
    }
    return result;
}
