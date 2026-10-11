#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound-novel opcode: start a countdown of the 16-bit operand in seconds
 * (60 frames each); when it runs out the script branches to the section in
 * the top operand byte. */
void wldcore_opcode_start_countdown_timer(void) {
    u32 instruction;

    instruction = g_wldcore_opcode_state.instruction;
    g_wldcore_sound_novel_countdown_elapsed = 0;
    g_wldcore_sound_novel_countdown_frames = ((instruction << 8) >> 16) * 60;
    g_wldcore_sound_novel_countdown_section = instruction >> 24;
    g_wldcore_state_flags |= WLDCORE_OPCODE_STATE_FETCH;
}
