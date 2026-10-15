#include "fft/wldcore.h"
#include "psx/types.h"

/* Pack a calendar date (the month in source[0], the day in source[1]) as record `record` of a bit
 * array of 9-bit records: the day in bits 0-4, the month in bits 5-8. The dates treasures and
 * unexplored lands were found and propositions last succeeded are kept this way. */
void wldcore_calendar_write_date(u32* bits, s32 record, u8* source) {
    s32 base = record * 9;
    s32 mask;
    s32 i;

    mask = 1;
    for (i = 0; i < 5; i++) {
        wldcore_bits_assign(bits, base + i, source[1] & mask);
        mask <<= 1;
    }
    mask = 1;
    for (i = 0; i < 4; i++) {
        /* The separate temporary keeps the target's add order: folding
         * base + (i + 5) in one expression hoists base + 5 out of the loop. */
        s32 index = i + 5;

        wldcore_bits_assign(bits, base + index, source[0] & mask);
        mask <<= 1;
    }
}
