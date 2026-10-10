#include "fft/event_bunit.h"
#include "psx/types.h"

/* Returns 1 for an ability of any type other than ABILITY_TYPE_DEFAULT (by
 * the type the "learned" callback cached) whose entry has bit 14 or 15 set;
 * 0 otherwise. */
s32 bunit_ability_is_non_action_unlearned(s32 index) {
    s32 result;

    result = 0;
    if (g_bunit_ability_type != ABILITY_TYPE_DEFAULT) {
        result = (g_bunit_ability_entries[index] >> ABILITY_LIST_ENTRY_DISABLED_SHIFT) != 0;
    }
    return result;
}
