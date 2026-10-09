#include "fft/event_jobstts.h"
#include "psx/types.h"

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
