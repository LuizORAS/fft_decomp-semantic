#include "psx/libapi.h"
s32 SetRCnt(u32 spec, u16 target, s32 mode) {
    s32 counter = spec & PSYQ_RCNT_SPEC_MASK;
    s32 hardware_mode = PSYQ_RCNT_BASE_MODE;
    if (counter >= PSYQ_RCNT_HARDWARE_COUNT) {
        return 0;
    }
    g_psyq_api_root_counters[counter].mode = 0;
    g_psyq_api_root_counters[counter].target = target;
    /* The unsigned comparison preserves the original SLTIU. */
    if ((u32)counter < 2) {
        if (mode & PSYQ_RCNT_API_SYNC_ENABLE) {
            hardware_mode |= PSYQ_RCNT_SYNC_ENABLE;
        }
        if (!(mode & PSYQ_RCNT_API_SYSTEM_CLOCK)) {
            hardware_mode |= PSYQ_RCNT_CLOCK_ALTERNATE;
        }
    } else if (counter == 2) {
        if (!(mode & PSYQ_RCNT_API_SYSTEM_CLOCK)) {
            hardware_mode |= PSYQ_RCNT_CLOCK_DIV8;
        }
    }
    if (mode & PSYQ_RCNT_API_IRQ_ENABLE) {
        hardware_mode |= PSYQ_RCNT_IRQ_ON_TARGET;
    }
    g_psyq_api_root_counters[counter].mode = hardware_mode;
    return 1;
}
