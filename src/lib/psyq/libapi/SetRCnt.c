#include "psx/libapi.h"

/* Program root counter `spec` (its low 16 bits select 0-2) to count up to `target` and start over, with an
 * interrupt at each target when `mode` has 0x1000. Counters 0 and 1 count their other clock (the dot clock,
 * the horizontal blank) unless `mode` has 0x1, and set the hardware sync bit when it has 0x10; counter 2
 * counts the system clock / 8 unless `mode` has 0x1. Returns 1, or 0 for a counter past 2.
 * suzuki_sound_init sets counter 2 to 0x44e8 at the system clock / 8, about 240 interrupts a second: the
 * sound driver's tick. */
s32 SetRCnt(u32 spec, u16 target, s32 mode) {
    s32 counter = spec & PSYQ_RCNT_SPEC_MASK;
    u16 hardware_mode = PSYQ_RCNT_BASE_MODE;
    if (counter >= PSYQ_RCNT_HARDWARE_COUNT) {
        return 0;
    }
    g_psyq_api_root_counters[counter].mode = 0;
    g_psyq_api_root_counters[counter].target = target;
    if (counter == 0 || counter == 1) {
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
