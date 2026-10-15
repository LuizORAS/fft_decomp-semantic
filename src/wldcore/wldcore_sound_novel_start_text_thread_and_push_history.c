#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 3: show text operand 0 (TEXT_ID_WORLD_MESSAGE_BASE + operand, dialog type 9) in the
 * message thread, as opcode 13 does, and push it onto the record's four-entry text history, which the
 * history level pages back through. */
void wldcore_sound_novel_start_text_thread_and_push_history(void) {
    u16 history_1;
    u16 history_2;
    u16 history_3;
    u16 flags;
    s32 operand;

    operand = g_wldcore_sound_novel_record.instruction.bytes.operand_0;
    world_thread_start(WLDCORE_THREAD_SLOT_MESSAGE, world_text_character_handling_thread);
    world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 9, operand | TEXT_ID_WORLD_MESSAGE_BASE, 0);
    history_1 = g_wldcore_sound_novel_record.text_history_1;
    history_2 = g_wldcore_sound_novel_record.text_history_2;
    history_3 = g_wldcore_sound_novel_record.text_history_3;
    flags = g_wldcore_sound_novel_record.state_flags;
    g_wldcore_sound_novel_record.text_id = operand;
    g_wldcore_sound_novel_record.text_history_3 = operand;
    g_wldcore_sound_novel_record.text_history_0 = history_1;
    g_wldcore_sound_novel_record.text_history_1 = history_2;
    g_wldcore_sound_novel_record.text_history_2 = history_3;
    g_wldcore_sound_novel_record.state_flags = flags | WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
