#include "fft/world.h"
#include "psx/types.h"

/* Caching a view across selector tests removes the native script-base reload. */

/* Measure the widest text a DisplayMessage window will show.
 *
 * With flag 0x80 set, the message at `offset` is measured, then every later
 * ChangeDialog (0x51) whose window mask (operand byte 1) covers `flags & 3`
 * contributes its text width, until a ChangeDialog with text id 0xffff closes
 * that window. The result is the widest measured text. This is the WORLD copy
 * of battle_text_build_display_message. */
void world_text_build_display_message(s32 offset, s32 flags, s32* out_width) {
    s16 width;
    s16 rows;
    s32 max_width;
    s32 next;
    s32 size;

    if (flags & DIALOG_TYPE_NO_BOX) {
        flags &= DIALOG_TYPE_POSITION_MASK;
        text_measure_pixels(&width, &rows,
            text_find_entry(((event_display_message_at(g_world_event_script, offset)->text_id_le[1] << 8)
                                + event_display_message_at(g_world_event_script, offset)->text_id_le[0])
                - 1));
        max_width = width;
        next = offset + 1;
        size = g_world_event_instruction_sizes[EVENT_OPCODE_DISPLAY_MESSAGE];
        offset = next + size;
        while ((offset = world_script_find_instruction_byte_offset(offset, EVENT_OPCODE_CHANGE_DIALOG)) != 0) {
            if (event_change_dialog_at(g_world_event_script, offset)->opcode == EVENT_OPCODE_CHANGE_DIALOG) {
                if (event_change_dialog_at(g_world_event_script, offset)->text_id_le[0] == 0xFF
                    && event_change_dialog_at(g_world_event_script, offset)->text_id_le[1] == 0xFF
                    && (event_change_dialog_at(g_world_event_script, offset)->window_selector & flags) == flags) {
                    break;
                }
                if ((event_change_dialog_at(g_world_event_script, offset)->window_selector & flags) == flags) {
                    text_measure_pixels(&width, &rows,
                        text_find_entry(((event_change_dialog_at(g_world_event_script, offset)->text_id_le[1] << 8)
                                            + event_change_dialog_at(g_world_event_script, offset)->text_id_le[0])
                            - 1));
                    if (max_width < width) {
                        max_width = width;
                    }
                }
            }
            next = offset + 1;
            size = g_world_event_instruction_sizes[EVENT_OPCODE_CHANGE_DIALOG];
            offset = next + size;
        }
        *out_width = max_width;
    }
}
