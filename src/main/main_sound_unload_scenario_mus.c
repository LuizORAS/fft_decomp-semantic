#include "fft/main.h"
#include "psx/types.h"

/* Frees the music record and SMD data of a slot. It does not clear the
 * current-music state when the slot is the playing one. */
s32 main_sound_unload_scenario_mus(s32 slot) {
    if (g_main_sound_music.slots.smd[slot] != 0) {
        SuzukiUnloadMUS(g_main_sound_music.slots.handles.music[slot]);
        g_main_sound_music.slots.handles.music[slot] = 0;
        main_heap_free_smd(g_main_sound_music.slots.smd[slot]);
        g_main_sound_music.slots.smd[slot] = 0;
        return 1;
    }
    return 0;
}
