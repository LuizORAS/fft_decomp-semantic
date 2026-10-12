#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 53: push operand 0 onto the record's four-entry text history without showing
 * it (the same shift as wldcore_sound_novel_start_text_thread_and_push_history). */
void wldcore_sound_novel_push_text_history(void) {
    u16 history_1;
    u8 new_entry;
    u16 history_2;
    u16 history_3;

    history_1 = g_wldcore_sound_novel_text_history_1;
    new_entry = g_wldcore_sound_novel_record.instruction.bytes.operand_0;
    history_2 = g_wldcore_sound_novel_text_history_2;
    history_3 = g_wldcore_sound_novel_text_history_3;
    g_wldcore_sound_novel_text_history_0 = history_1;
    g_wldcore_sound_novel_text_history_1 = history_2;
    g_wldcore_sound_novel_text_history_2 = history_3;
    g_wldcore_sound_novel_text_history_3 = new_entry;
    g_wldcore_sound_novel_state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
