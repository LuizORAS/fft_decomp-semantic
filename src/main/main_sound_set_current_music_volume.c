#include "fft/main.h"
#include "psx/types.h"

/* Fades the current music to volume over time ticks without remembering
 * the volume. */
int main_sound_set_current_music_volume(int volume, int time) {
    suzuki_music_t* music = g_main_sound_music.state.current_music;

    if (music != 0) {
        suzuki_smd_set_music_volume(music, volume, time);
        return 1;
    }
    return 0;
}
