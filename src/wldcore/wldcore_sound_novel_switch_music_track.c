#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 39: when a track is selected, switch playback to its slot at volume 0 and fade
 * to the queue's volume over operand 0 * 3 steps. */
void wldcore_sound_novel_switch_music_track(void) {
    s16 track = g_wldcore_sound_novel_record.music_selection;
    s32* state;
    s32 operand;

    if (track != 0) {
        state = &g_wldcore_audio_queue.music.flags;
        *state |= WLDCORE_AUDIO_MUSIC_PLAYING;
        operand = g_wldcore_sound_novel_record.instruction.bytes.operand_0;
        main_sound_switch_music_track(track >> 8, 0, 0);
        main_sound_set_current_music_target(g_wldcore_audio_queue.volume, operand * 3);
    }
    g_wldcore_sound_novel_record.state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
