#include "fft/main.h"

/* Finish the load main_sound_start_music_load_into_free_slot started: return 1 while the read for slot
 * handles.loading_slot runs; once it is done, create that slot's music record (SuzukiPutPlaySMD), clear
 * loading_slot and return 0. Returns 0 at once when no load is in flight. */
int main_sound_poll_music_load(void) {
    if (g_main_sound_music.slots.handles.loading_slot == 0) {
        return 0;
    }
    if (main_file_is_still_loading() != 0) {
        return 1;
    }
    g_main_sound_music.slots.handles.music[g_main_sound_music.slots.handles.loading_slot]
        = SuzukiPutPlaySMD(g_main_sound_music.slots.smd[g_main_sound_music.slots.handles.loading_slot]);
    g_main_sound_music.slots.handles.loading_slot = 0;
    return 0;
}
