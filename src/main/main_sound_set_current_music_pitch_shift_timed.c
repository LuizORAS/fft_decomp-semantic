#include "fft/main.h"

/* Shift the pitch of the current scenario music to `value`, at once when `time` is 0 or as a ramp over
 * `time` ticks (suzuki_smd_set_pitch_shift); return 1, or 0 when no music is playing. No retail code calls
 * it. */
int main_sound_set_current_music_pitch_shift_timed(int value, int time) {
    if (g_main_sound_music.state.current_music != 0) {
        suzuki_smd_set_pitch_shift(g_main_sound_music.state.current_music, value, time);
        return 1;
    }
    return 0;
}
