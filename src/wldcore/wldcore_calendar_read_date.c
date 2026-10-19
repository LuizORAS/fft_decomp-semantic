#include "fft/wldcore.h"
#include "psx/types.h"

/* Unpack record `record` of a bit array of 9-bit dates (wldcore_calendar_write_date): bits 0-4 into
 * dest->day, bits 5-8 into dest->month; a field that reads 0 or out of range (a day of 32 or more, a
 * month of 13 or more) comes back as 1. */
void wldcore_calendar_read_date(u32* bits, s32 record, calendar_date_t* dest) {
    s32 base = record * 9;
    s32 mask;
    s32 i;

    mask = 1;
    dest->day = 0;
    for (i = 0; i < 5; i++) {
        if (wldcore_bits_test(bits, base + i)) {
            dest->day |= mask;
        }
        mask <<= 1;
    }
    mask = 1;
    dest->month = 0;
    for (i = 0; i < 4; i++) {
        s32 index = i + 5;

        if (wldcore_bits_test(bits, base + index)) {
            dest->month |= mask;
        }
        mask <<= 1;
    }
    if (dest->day == 0 || dest->day >= 32) {
        dest->day = 1;
    }
    if (dest->month == 0 || dest->month >= 13) {
        dest->month = 1;
    }
}
