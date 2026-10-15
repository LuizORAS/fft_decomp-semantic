#include "fft/main.h"
#include "psx/types.h"

/* Same as main_sound_set_current_music_pitch_shift_timed for the tempo scale, 0x100 = 1.0
 * (main_smd_set_tempo_scale). No retail code calls it. */
int main_sound_set_current_music_tempo_scale_timed(int value, int time) {
    suzuki_music_t* music = g_main_sound_music.state.current_music;

    if (music != 0) {
        main_smd_set_tempo_scale(music, value, time);
        return 1;
    }
    return 0;
}
