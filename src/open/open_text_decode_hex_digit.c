#include "psx/types.h"

s32 open_text_decode_hex_digit(s32 character) {
    u32 value;

    value = character - '0';
    if (value < 10) {
        return value;
    }

    if (character >= 'A' && character < 'G') {
        return character - ('A' - 10);
    }

    if (character >= 'a' && character < 'g') {
        return character - ('a' - 10);
    }

    return 0;
}
