#include "psx/libapi.h"

/* Return root counter `spec`'s current count, or 0 for a counter past 2. No code refers to it. */
s32 GetRCnt(u32 spec) {
    s32 counter = spec & PSYQ_RCNT_SPEC_MASK;
    if (counter >= PSYQ_RCNT_HARDWARE_COUNT) {
        return 0;
    }
    return g_psyq_api_root_counters[counter].count;
}
