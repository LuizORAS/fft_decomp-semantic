#include "fft/main.h"

/* Starts the remembered background sound again. */
void main_sound_replay_bg_sfx(void) {
    int sound_id = g_main_sound_weather_sfx_id;

    if (sound_id != 0) {
        SuzukiPlaySound2(sound_id);
    }
}
