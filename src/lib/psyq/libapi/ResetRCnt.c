#include "psx/libapi.h"

/* Set root counter `spec`'s count to 0 and return 1, or 0 for a counter past 2. No code refers to it. */
s32 ResetRCnt(u32 spec) {
    s32 counter = spec & PSYQ_RCNT_SPEC_MASK;
    if (counter >= PSYQ_RCNT_HARDWARE_COUNT) {
        return 0;
    }
    g_psyq_api_root_counters[counter].count = 0;
    return 1;
}
