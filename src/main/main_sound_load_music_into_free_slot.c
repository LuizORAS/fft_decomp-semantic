#include "fft/main.h"

/* Same as main_sound_load_music_into_slot for the first free scenario slot (1 or 2); returns 0 when both
 * are taken. The shop, formation and save screens, the music test and main_sound_set_current_music_track
 * open their music this way. */
int main_sound_load_music_into_free_slot(int music_id) {
    int i;
    suzuki_smd_header_t* smd;

    for (i = 0; i < 2; i++) {
        if (g_main_sound_music.slots.smd[i + 1] == 0) {
            smd = main_file_alloc_smd_and_load(
                g_main_sound_scenario_smd_files[music_id].sector, g_main_sound_scenario_smd_files[music_id].size);
            g_main_sound_music.slots.smd[i + 1] = smd;
            g_main_sound_music.slots.handles.music[i + 1] = SuzukiPutPlaySMD(smd);
            return i + 1;
        }
    }
    return 0;
}
