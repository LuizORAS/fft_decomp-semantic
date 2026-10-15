#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 44: stop the background sound and play row operand 0 of the sound quad table,
 * four sounds of the novel's sound resource (wldcore_sound_stop_weather_sfx_and_send_quad). A nonzero
 * operand 1 also records the sound, so a restored reading replays it. */
void wldcore_sound_novel_play_sound_quad(void) {
    char unused[8];
    s32 sound_id;

    sound_id = g_wldcore_sound_novel_record.instruction.bytes.operand_0;
    if (g_wldcore_sound_novel_state.instruction.word & 0xFF0000) {
        g_wldcore_sound_novel_record.sound_id = sound_id;
    }
    wldcore_sound_stop_weather_sfx_and_send_quad(sound_id);
    g_wldcore_sound_novel_record.state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
