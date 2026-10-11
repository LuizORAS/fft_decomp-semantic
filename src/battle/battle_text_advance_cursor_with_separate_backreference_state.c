#include "psx/types.h"

/*
 * Three-pointer variant of battle_text_advance_cursor_with_backreferences
 * (0x80130718): the same text-stream walk, but the caller supplies the
 * remaining-byte counter and the resume cursor separately instead of a state
 * record. `scratch` plays the role of that record's scratch word: first the
 * high bits of the byte count, then the back-offset.
 */
u8* battle_text_advance_cursor_with_separate_backreference_state(s32* remaining_bytes, u8* cursor, u8** return_cursor) {
    s32 remaining;
    s32 scratch;
    u8 command;

    remaining = *remaining_bytes;
    cursor += 1;
    if (remaining > 0) {
        remaining -= 1;
        *remaining_bytes = remaining;
        if (remaining == 0) {
            cursor = *return_cursor;
            *return_cursor = (u8*)-1;
        }
    }
    command = cursor[0];
    if (((command & 0xF0) == 0xF0) && ((command & 0xF) < 4)) {
        *return_cursor = cursor + 3;
        scratch = (cursor[0] & 3) << 3;
        *remaining_bytes = scratch + (cursor[1] >> 5) + 4;
        scratch = cursor[1] & 0xF;
        scratch = scratch * 0xFE + cursor[2];
        cursor -= scratch;
    }
    return cursor;
}
