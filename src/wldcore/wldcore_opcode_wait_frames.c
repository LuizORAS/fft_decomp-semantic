#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound-novel opcode: wait operand 0 frames. The first pass (state bit
 * 0x10) latches the frame count and clears the counter; each later pass
 * yields until the counter has passed the count, then asks for the next
 * instruction. */
void wldcore_opcode_wait_frames(void) {
    u16 flags;
    u16 updated;
    s32 counter;
    s32 operand;

    flags = g_wldcore_active_saved_record.state_flags;
    updated = flags | WLDCORE_OPCODE_STATE_YIELD;
    g_wldcore_active_saved_record.state_flags = updated;
    if ((updated & WLDCORE_OPCODE_STATE_FETCHED) != 0) {
        g_wldcore_sound_novel_wait_counter = 0;
        g_wldcore_sound_novel_wait_frames = (u8)(g_wldcore_active_saved_record.instruction >> 8);
    }
    counter = g_wldcore_sound_novel_wait_counter;
    operand = g_wldcore_sound_novel_wait_frames;
    if (operand < counter) {
        g_wldcore_active_saved_record.state_flags = flags | (WLDCORE_OPCODE_STATE_FETCH | WLDCORE_OPCODE_STATE_YIELD);
    } else {
        g_wldcore_sound_novel_wait_counter = counter + 1;
    }
}
