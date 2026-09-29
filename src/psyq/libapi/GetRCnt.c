#include "psx/libapi.h"
s32 GetRCnt(u32 spec) {
    s32 counter = spec & PSYQ_RCNT_SPEC_MASK;
    /* Removing this pin changes the original return and branch-delay sequence. */
    register s32 result __asm__("$2");
    if (counter < PSYQ_RCNT_HARDWARE_COUNT) {
        result = g_psyq_api_root_counters[counter].count;
    } else {
        result = 0;
    }
    return result;
}
