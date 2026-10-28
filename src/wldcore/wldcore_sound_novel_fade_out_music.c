#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 38: fade the music to silence over operand 0 * 3 steps and clear the playing
 * flag and the selected music id (the slot is kept). */
void wldcore_sound_novel_fade_out_music(void) {
    s32* state;
    s32 operand;

    state = &g_wldcore_audio_queue.music.flags;
    *state &= ~WLDCORE_AUDIO_MUSIC_PLAYING;
    operand = g_wldcore_sound_novel_record.instruction.bytes.operand_0;
    main_sound_set_current_music_target(0, operand * 3);
    g_wldcore_sound_novel_record.state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
    g_wldcore_sound_novel_record.music_selection &= 0xFF00;
}
