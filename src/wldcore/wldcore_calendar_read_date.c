#include "fft/wldcore.h"
#include "psx/types.h"

/* Unpack record `record` of a bit array of 9-bit dates (wldcore_calendar_write_date) into dest[0]
 * (the month) and dest[1] (the day); a field that reads 0 or out of range (a day of 32 or more, a
 * month of 13 or more) comes back as 1. */
void wldcore_calendar_read_date(u32* bits, s32 record, u8* dest) {
    s32 base = record * 9;
    s32 mask;
    s32 i;

    mask = 1;
    dest[1] = 0;
    for (i = 0; i < 5; i++) {
        if (wldcore_test_bit(bits, base + i)) {
            dest[1] |= mask;
        }
        mask <<= 1;
    }
    mask = 1;
    dest[0] = 0;
    for (i = 0; i < 4; i++) {
        s32 index = i + 5;

        if (wldcore_test_bit(bits, base + index)) {
            dest[0] |= mask;
        }
        mask <<= 1;
    }
    if (dest[1] == 0 || dest[1] >= 32) {
        dest[1] = 1;
    }
    if (dest[0] == 0 || dest[0] >= 13) {
        dest[0] = 1;
    }
}
