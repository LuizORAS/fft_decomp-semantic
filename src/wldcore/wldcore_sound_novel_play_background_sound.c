#include "fft/wldcore.h"

/* Sound novel opcode 47: play environment sound operand 0 as the background sound
 * (wldcore_sound_stop_music_and_play_sound). A nonzero operand 1 also records it, flagged 0x4000, so a
 * restored reading replays it. */
void wldcore_sound_novel_play_background_sound(void) {
    char unused[8];
    s32 sound_id;

    sound_id = g_wldcore_sound_novel_record.instruction.bytes.operand_0;
    if (g_wldcore_sound_novel_state.instruction.word & 0xFF0000) {
        g_wldcore_sound_novel_record.sound_id = sound_id | 0x4000;
    }
    wldcore_sound_stop_music_and_play_sound(sound_id);
    g_wldcore_sound_novel_record.state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
