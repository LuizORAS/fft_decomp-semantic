#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 32: change to scenario music operand 0 (wldcore_sound_change_scenario_music). */
void wldcore_sound_novel_play_music(void) {
    wldcore_sound_change_scenario_music(g_wldcore_sound_novel_record.instruction.bytes.operand_0);
    g_wldcore_sound_novel_record.state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
