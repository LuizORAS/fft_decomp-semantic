#include "fft/main.h"
#include "psx/types.h"

/* Stops the current music and restarts the music loaded in slot from its
 * beginning, fading in to volume over fade ticks. Returns the music record
 * as a handle, or 0 for slot 0. */
s32 main_sound_switch_music_track(s32 slot, s32 volume, s32 fade) {
    main_sound_stop_current_music();
    if (slot != 0) {
        if (g_main_sound_music.slots.smd[slot] != 0) {
            g_main_sound_music.state.current_slot = slot;
            g_main_sound_music.state.current_smd = g_main_sound_music.slots.smd[slot];
            g_main_sound_music.state.current_music = g_main_sound_music.slots.handles.music[slot];
            g_main_sound_music.state.volume = volume;
            main_smd_reset_music(g_main_sound_music.state.current_music, volume, fade);
        }
        return (s32)g_main_sound_music.state.current_music;
    }
    return 0;
}
