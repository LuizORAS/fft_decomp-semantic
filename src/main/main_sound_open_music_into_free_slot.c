#include "fft/main.h"

int main_sound_open_music_into_free_slot(int scenario) {
    int i;
    suzuki_smd_header_t* smd;

    for (i = 0; i < 2; i++) {
        if (g_main_sound_music.slots.smd[i + 1] == 0) {
            smd = main_file_get_smd(
                g_main_sound_scenario_smd_files[scenario].sector, g_main_sound_scenario_smd_files[scenario].size);
            g_main_sound_music.slots.smd[i + 1] = smd;
            g_main_sound_music.slots.handles.music[i + 1] = SuzukiPutPlaySMD(smd);
            return i + 1;
        }
    }
    return 0;
}
