#include "fft/menu.h"
#include "psx/types.h"

/* Advance a text cursor by one byte, leaving a finished back-reference run and
 * entering a new one. Several modules compile this text, and BATTLE and WORLD compile it again
 * (menu_text_advance_cursor_with_backreferences) beside battle_menu_display_text and
 * world_menu_display_text, each copy's only caller.
 *
 * A back-reference is a three-byte command 0xF0-0xF3 that replays earlier text (4 to 35 bytes):
 * the run length is (byte 0 & 3) * 8 + (byte 1 >> 5) + 4 bytes, and it starts
 * (byte 1 & 0xf) * 0xfe + byte 2 bytes before the command. After the run the
 * cursor returns to the byte following the command. */
u8* text_advance_cursor_with_backreferences(world_text_backreference_state_t* state, u8* cursor) {
    text_backref_t* backref;
    s32 remaining;
    u8 command;

    remaining = state->remaining_bytes;
    cursor += 1;
    if (remaining > 0) {
        remaining -= 1;
        state->remaining_bytes = remaining;
        if (remaining == 0) {
            cursor = state->return_cursor;
            state->return_cursor = (u8*)-1;
        }
    }
    command = cursor[0];
    /* Commands 0xF0..0xF3 jump the cursor backwards for a fixed number of
     * bytes, then resume after the 3-byte command. */
    if (((command & 0xF0) == TEXT_BACKREFERENCE_PREFIX_FIRST) && ((command & 0xF) < 4)) {
        state->return_cursor = cursor + 3;
        backref = (text_backref_t*)cursor;
        state->scratch = backref->length_high * 8;
        state->remaining_bytes = backref->length_low;
        state->remaining_bytes = state->remaining_bytes + state->scratch + 4;
        state->scratch = backref->offset_high * 0xFE;
        state->scratch = backref->offset_low + state->scratch;
        cursor -= state->scratch;
    }
    return cursor;
}
