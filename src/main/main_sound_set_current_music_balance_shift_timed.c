#include "fft/main.h"

/* Same as main_sound_set_current_music_pitch_shift_timed for the balance shift
 * (suzuki_smd_set_balance_shift). No retail code calls it. */
int main_sound_set_current_music_balance_shift_timed(int value, int time) {
    if (g_main_sound_music.state.current_music != 0) {
        suzuki_smd_set_balance_shift(g_main_sound_music.state.current_music, value, time);
        return 1;
    }
    return 0;
}
