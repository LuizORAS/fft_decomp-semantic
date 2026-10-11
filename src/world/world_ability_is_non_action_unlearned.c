#include "fft/world.h"
#include "psx/types.h"

/* For a cached ability outside the default class (an item, Throw, Jump, Charge, Math Skill or R/S/M
 * ability), report whether list entry index carries a flag in bits 14-15
 * (ABILITY_LIST_ENTRY_DISABLED); 0 for a default ability. Menu script callback 14 of the view and
 * learn lists. */
s32 world_ability_is_non_action_unlearned(s32 index) {
    s32 result = 0;

    if (g_world_ability_selected_type != 0) {
        result = (g_world_ability_entries[index] >> ABILITY_LIST_ENTRY_DISABLED_SHIFT) != 0;
    }
    return result;
}
