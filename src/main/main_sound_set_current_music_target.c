#include "fft/main.h"
#include "psx/types.h"

/* Fades the current music to volume over time ticks and remembers the
 * volume for main_sound_set_current_music_volume_timed. */
int main_sound_set_current_music_target(int volume, int time) {
    suzuki_music_t* music = g_main_sound_music.state.current_music;

    if (music != 0) {
        g_main_sound_music.state.volume = volume;
        suzuki_smd_set_music_volume(music, volume, time);
        return 1;
    }
    return 0;
}
