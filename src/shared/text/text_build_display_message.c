#include "fft/battle_text.h"
#include "fft/menu.h"
#include "fft/script.h"

/* Measure the widest text a DisplayMessage window will show. BATTLE and WORLD compile this text.
 *
 * With flag 0x80 set, the message at `offset` is measured, then every later
 * ChangeDialog (0x51) whose window mask (operand byte 1) covers `flags & 3`
 * contributes its text width, until a ChangeDialog with text id 0xffff closes
 * that window. The result is the widest measured text. */
void text_build_display_message(s32 offset, s32 flags, s32* out_width) {
    s16 width;
    s16 height;
    s32 max_width;
    s32 next;
    s32 size;

    if (flags & DIALOG_TYPE_NO_BOX) {
        flags &= DIALOG_TYPE_POSITION_MASK;
        text_measure_pixels(&width, &height,
            text_find_entry(((event_display_message_at((u8*)g_script_event_block, offset)->text_id_le[1] << 8)
                                + event_display_message_at((u8*)g_script_event_block, offset)->text_id_le[0])
                - 1));
        max_width = width;
        next = offset + 1;
        size = g_script_event_instruction_sizes[EVENT_OPCODE_DISPLAY_MESSAGE];
        offset = next + size;
        while ((offset = script_find_instruction_byte_offset(offset, EVENT_OPCODE_CHANGE_DIALOG)) != 0) {
            if (EVENT_BYTE((u8*)g_script_event_block, offset)->opcode == EVENT_OPCODE_CHANGE_DIALOG) {
                if (EVENT_BYTE((u8*)g_script_event_block, offset)->text_id_le[0] == 0xFF
                    && EVENT_BYTE((u8*)g_script_event_block, offset)->text_id_le[1] == 0xFF
                    && (EVENT_BYTE((u8*)g_script_event_block, offset)->window_selector & flags) == flags) {
                    break;
                }
                if ((EVENT_BYTE((u8*)g_script_event_block, offset)->window_selector & flags) == flags) {
                    text_measure_pixels(&width, &height,
                        text_find_entry(((EVENT_BYTE((u8*)g_script_event_block, offset)->text_id_le[1] << 8)
                                            + EVENT_BYTE((u8*)g_script_event_block, offset)->text_id_le[0])
                            - 1));
                    if (max_width < width) {
                        max_width = width;
                    }
                }
            }
            next = offset + 1;
            size = g_script_event_instruction_sizes[EVENT_OPCODE_CHANGE_DIALOG];
            offset = next + size;
        }
        *out_width = max_width;
    }
}
