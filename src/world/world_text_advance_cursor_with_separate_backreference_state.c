#include "fft/world.h"
#include "psx/types.h"

/* `nibble` carries the opcode test and then the displacement nibble: sharing one
 * variable across the two blocks makes it a global allocno, so local_alloc
 * cannot tie the masked value to the dying byte load.  `value` likewise holds the
 * product, which keeps the final addu's result in the displacement register. */

/* Advance a byte-coded script cursor by one byte, honouring a pending
 * repeat counter (*count) whose resume address is *saved.  A repeat opcode
 * at the new position starts a new repeat: it stores the resume address,
 * derives the repeat count and rewinds by the packed displacement. */
u8* world_text_advance_cursor_with_separate_backreference_state(s32* count, u8* cursor, u8** saved) {
    s32 value = *count;
    s32 nibble;

    cursor += 1;
    if (value > 0) {
        value -= 1;
        *count = value;
        if (value == 0) {
            cursor = *saved;
            *saved = (u8*)-1;
        }
    }
    nibble = cursor[0] & 0xF0;
    if (nibble == 0xF0 && (cursor[0] & 0xF) < 4) {
        *saved = cursor + 3;
        value = ((cursor[0] & 3) << 3) + (cursor[1] >> 5);
        *count = value + 4;
        nibble = cursor[1] & 0xF;
        value = nibble * 254;
        cursor -= value + cursor[2];
    }
    return cursor;
}
