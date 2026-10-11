#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound-novel opcode: when a track is selected, switch playback to its slot
 * at volume 0 and fade to the queue's volume over operand 0 * 3 steps. */
void wldcore_opcode_switch_music_track(void) {
    s16 track = g_wldcore_active_saved_record.music_selection;
    s32* state;
    s32 operand;

    if (track != 0) {
        state = &g_wldcore_audio_queue.music.flags;
        *state |= WLDCORE_AUDIO_MUSIC_PLAYING;
        operand = (u8)(g_wldcore_active_saved_record.instruction >> 8);
        main_sound_switch_music_track(track >> 8, 0, 0);
        main_sound_set_current_music_target(g_wldcore_audio_queue.volume, operand * 3);
    }
    g_wldcore_active_saved_record.state_flags |= WLDCORE_OPCODE_STATE_FETCH;
}
