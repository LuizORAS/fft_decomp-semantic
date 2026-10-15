#include "psx/types.h"

/* Fill a 16-byte name field with the terminator 0xFE; same as battle_text_fill_field_with_terminators. */
void world_text_fill_field_with_terminators(u8* buf) {
    s32 value = 0xFE;
    s32 i = 0xF;

    do {
        *buf = value;
        i -= 1;
        buf += 1;
    } while (i >= 0);
}
