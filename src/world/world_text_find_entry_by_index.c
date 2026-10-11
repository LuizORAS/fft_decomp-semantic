#include "fft/world.h"
#include "psx/types.h"

/* Advance past `count` entry terminators in the encoded menu-text stream. */
u8* world_text_find_entry_by_index(u8* text, s16 count, s32 unused) {
    u8 code;

    while (count != 0) {
        code = *text++;
        if (code >= TEXT_EXTENDED_GLYPH_PREFIX_FIRST) {
            if (code < TEXT_FORMAT_FIRST) {
                text++;
            } else if (code >= TEXT_END_WAIT_FOR_CONFIRM) {
                count--;
            }
        }
    }
    return text;
}
