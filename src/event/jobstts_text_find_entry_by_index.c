#include "fft/event_jobstts.h"
#include "psx/types.h"

/* Advance past `terminators` text terminators (0xfe or 0xff), stepping over the
 * second byte of two-byte glyphs, and return the entry that follows. The
 * third argument is not read. */
const u8* jobstts_text_find_entry_by_index(const u8* text, s16 terminators, s32 unused_2) {
    while (terminators != 0) {
        u8 character = *text++;

        if (character >= TEXT_EXTENDED_GLYPH_PREFIX_FIRST) {
            if (character <= TEXT_EXTENDED_GLYPH_PREFIX_LAST) {
                text++;
            } else if (character >= TEXT_END_WAIT_FOR_CONFIRM) {
                terminators--;
            }
        }
    }
    return text;
}
