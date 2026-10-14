#include "fft/world.h"
#include "psx/types.h"

/* Skip `count` entries of a text section and return the next one: each entry ends at 0xFE or 0xFF, and
 * a 0xD0-0xDF byte takes the following byte with it. It does not follow back-references; the third argument
 * is not read. */
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
