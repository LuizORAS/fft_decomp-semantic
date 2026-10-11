#include "fft/main.h"
#include "psx/types.h"

/* Fades the current music back to the remembered volume over time ticks. */
int main_sound_set_current_music_volume_timed(int time) {
    suzuki_music_t* music = g_main_sound_music.state.current_music;

    if (music != 0) {
        suzuki_smd_set_music_volume(music, g_main_sound_music.state.volume, time);
        return 1;
    }
    return 0;
}
