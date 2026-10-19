#include "fft/menu.h"
#include "psx/types.h"

/* Advance a text cursor by one byte as text_advance_cursor_with_backreferences does, keeping the
 * run's remaining byte count in *remaining_bytes and its return point in *return_cursor instead of a state
 * record: the form the dialog thread uses (battle_text_run_dialog_thread, and
 * world_text_character_handling_thread in WORLD's copy). `scratch` plays the record's 32-bit scratch value:
 * first the high bits of the byte count, then the back-offset. */
u8* text_advance_cursor_with_separate_backreference_state(s32* remaining_bytes, u8* cursor, u8** return_cursor) {
    text_backref_t* backref;
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
    if (((command & 0xF0) == TEXT_BACKREFERENCE_PREFIX_FIRST) && ((command & 0xF) < 4)) {
        *return_cursor = cursor + 3;
        backref = (text_backref_t*)cursor;
        scratch = backref->length_high << 3;
        *remaining_bytes = scratch + backref->length_low + 4;
        scratch = backref->offset_high;
        scratch = scratch * 0xFE + backref->offset_low;
        cursor -= scratch;
    }
    return cursor;
}
