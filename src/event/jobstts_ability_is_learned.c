#include "fft/event_jobstts.h"
#include "psx/types.h"

s32 jobstts_ability_is_learned(s32 index) {
    return (g_jobstts_ability_entries[index] & ABILITY_LIST_ENTRY_DISABLED) == 0;
}
