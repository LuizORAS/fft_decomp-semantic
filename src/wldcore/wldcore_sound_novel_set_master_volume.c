#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 48: set the master volume to operand 0 * 0x40 (0xff gives the maximum 0x3fff)
 * over operand 1 * 3 steps, at least one. */
void wldcore_sound_novel_set_master_volume(void) {
    s32 unused[2];
    s32 steps = g_wldcore_sound_novel_record.instruction.bytes.operand_1 * 3;
    s32 volume = (g_wldcore_sound_novel_state.instruction.word & 0xFF00) >> 2;

    if (steps == 0) {
        steps = 1;
    }
    if (volume == 0x3FC0) {
        volume = 0x3FFF;
    }
    suzuki_sound_set_master_volume(volume, steps);
    g_wldcore_sound_novel_record.state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
