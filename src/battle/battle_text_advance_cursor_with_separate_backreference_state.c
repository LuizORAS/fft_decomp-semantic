#include "psx/types.h"

/* Advance a text cursor by one byte as battle_text_advance_cursor_with_backreferences does, keeping the
 * run's remaining byte count in *remaining_bytes and its return point in *return_cursor instead of a state
 * record: the form the dialog thread (battle_text_character_handling_thread) uses. */
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
