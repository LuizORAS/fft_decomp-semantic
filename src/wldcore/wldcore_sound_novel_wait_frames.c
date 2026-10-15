#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 9: wait operand 0 frames. The first pass (WLDCORE_SOUND_NOVEL_STATE_FETCHED)
 * latches the frame count and clears the counter; each later pass yields until the counter has passed
 * the count, then asks for the next instruction. */
void wldcore_sound_novel_wait_frames(void) {
    u16* state_flags;
    u16 flags;
    u16 updated;
    s32 counter;
    s32 operand;

    state_flags = &g_wldcore_sound_novel_record.state_flags;
    flags = *state_flags;
    updated = flags | WLDCORE_SOUND_NOVEL_STATE_YIELD;
    *state_flags = updated;
    if ((updated & WLDCORE_SOUND_NOVEL_STATE_FETCHED) != 0) {
        g_wldcore_sound_novel_wait_counter = 0;
        g_wldcore_sound_novel_wait_frames = g_wldcore_sound_novel_record.instruction.bytes.operand_0;
    }
    counter = g_wldcore_sound_novel_wait_counter;
    operand = g_wldcore_sound_novel_wait_frames;
    if (operand < counter) {
        *state_flags = flags | (WLDCORE_SOUND_NOVEL_STATE_FETCH | WLDCORE_SOUND_NOVEL_STATE_YIELD);
    } else {
        g_wldcore_sound_novel_wait_counter = counter + 1;
    }
}
