#include "fft/event_bunit.h"
#include "psx/types.h"

s32 bunit_ability_is_learned(s32 index) {
    /* The unsigned cast preserves the target's logical shift without sign extension. */
    return (((u16)g_bunit_ability_entries[index] >> ABILITY_LIST_ENTRY_DISABLED_SHIFT) ^ 1) & 1;
}
