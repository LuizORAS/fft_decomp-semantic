#include "fft/event_card.h"
#include "psx/types.h"

/* Advance past `count` text terminators (0xfe or 0xff), stepping over the
 * second byte of two-byte glyphs, and return the entry that follows. The
 * third argument is not read. */
const u8* card_text_find_entry_by_index(const u8* data, s16 count, s32 unused_2) {
    if (count != 0) {
        do {
            u32 raw = *data++;
            u32 value = raw & 0xff;

            if (value >= TEXT_EXTENDED_GLYPH_PREFIX_FIRST) {
                if (value <= TEXT_EXTENDED_GLYPH_PREFIX_LAST) {
                    data++;
                } else if (value >= TEXT_END_WAIT_FOR_CONFIRM) {
                    count--;
                }
            }
        } while (count != 0);
    }
    return data;
}
