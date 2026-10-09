#include "fft/main.h"
#include "psx/types.h"

int main_sound_set_tune_volume(int volume) {
    int tune = g_main_sound_music.slots.tune;

    if (tune != 0) {
        if ((tune & 0x80) != 0) {
            g_main_sound_music.slots.tune = 0;
        } else {
            SuzukiCalcMusVolChange(g_main_sound_music.slots.handles.music[2 + (tune & 0x3f)], 0, volume);
        }
    }
    return 1;
}
