#include "fft/thread.h"
#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 13: show text operand 0 (TEXT_ID_WORLD_MESSAGE_BASE + operand) in the message
 * thread with dialog type 9, and record it as the current text. */
void wldcore_sound_novel_start_text_thread(void) {
    s32 operand;

    operand = g_wldcore_sound_novel_record.instruction.bytes.operand_0;
    thread_start(WLDCORE_THREAD_SLOT_MESSAGE, world_text_character_handling_thread);
    thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 9, operand | TEXT_ID_WORLD_MESSAGE_BASE, 0);
    g_wldcore_sound_novel_record.text_id = operand;
    g_wldcore_sound_novel_record.state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
