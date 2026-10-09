#include "fft/main.h"

/*
 * handles.loading_slot holds the slot whose SMD load is in flight;
 * handles.music[1..2] hold the resulting music records and smd[1..2] their
 * source SMD data.
 */
int main_sound_poll_scenario_smd_load(void) {
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
