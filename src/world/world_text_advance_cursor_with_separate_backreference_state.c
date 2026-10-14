#include "psx/types.h"

/* `nibble` carries the opcode test and then the displacement nibble: sharing one
 * variable across the two blocks makes it a global allocno, so local_alloc
 * cannot tie the masked value to the dying byte load.  `value` likewise holds the
 * product, which keeps the final addu's result in the displacement register. */

/* A back-reference: the command 0xF0-0xF3 and two operand bytes (bit 4 of the first is unused). */
typedef struct {
    u8 count_hi : 2;
    u8 : 2;
    u8 opcode : 4;
    u8 disp_hi : 4;
    u8 : 1;
    u8 count_lo : 3;
    u8 disp_lo;
} world_text_backreference_t;

/* Same as battle_text_advance_cursor_with_separate_backreference_state: advance a text cursor by one
 * byte, with the back-reference run's remaining byte count in *count and its return point in *saved, the
 * form the dialog thread (world_text_character_handling_thread) uses. */
u8* world_text_advance_cursor_with_separate_backreference_state(s32* count, u8* cursor, u8** saved) {
    s32 value = *count;
    s32 nibble;
    world_text_backreference_t* op;

    cursor += 1;
    if (value > 0) {
        value -= 1;
        *count = value;
        if (value == 0) {
            cursor = *saved;
            *saved = (u8*)-1;
        }
    }
    op = (world_text_backreference_t*)cursor;
    nibble = cursor[0] & 0xF0;
    if (nibble == 0xF0 && (cursor[0] & 0xF) < 4) {
        *saved = cursor + 3;
        value = (op->count_hi << 3) + op->count_lo;
        *count = value + 4;
        nibble = op->disp_hi;
        value = nibble * 254;
        cursor -= value + op->disp_lo;
    }
    return cursor;
}
