#include "psx/types.h"

s32 card_text_count_encoded_glyphs(const u8* data) {
    s32 count = 0;

    if (*data != 0xfe) {
        do {
            if (*data >= 0xd0 && *data <= 0xdf) {
                data += 2;
            } else {
                data++;
            }
            count++;
        } while (*data != 0xfe);
    }
    return count;
}
