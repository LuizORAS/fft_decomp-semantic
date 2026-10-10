#include "fft/event_bunit.h"
#include "psx/types.h"

/* Menu callback: 1 unless the entry's disabled bit (0x4000) is set. */
s32 bunit_ability_is_learned(s32 index) {
    return (g_bunit_ability_entries[index] & ABILITY_LIST_ENTRY_DISABLED) == 0;
}
