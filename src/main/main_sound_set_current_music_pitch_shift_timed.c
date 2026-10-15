#include "fft/main.h"
#include "psx/types.h"

/* Shift the pitch of the current scenario music to `value`, at once when `time` is 0 or as a ramp over
 * `time` ticks (main_smd_set_pitch_shift); return 1, or 0 when no music is playing. No retail code calls
 * it. */
int main_sound_set_current_music_pitch_shift_timed(int value, int time) {
    suzuki_music_t* music = g_main_sound_music.state.current_music;

    if (music != 0) {
        main_smd_set_pitch_shift(music, value, time);
        return 1;
    }
    return 0;
}
