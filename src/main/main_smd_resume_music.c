#include "fft/main.h"
#include "psx/libapi.h"
#include "psx/libetc.h"
#include "psx/types.h"

/* Resumes a music handle paused by main_smd_pause_music: with the
 * root-counter event disabled it fades the master volume in from 0,
 * re-applies the song's reverb mode, forces every channel's voice
 * parameters, retriggers held voices, then clears the pause bit 0x100 and
 * marks the record playing. */
void main_smd_resume_music(suzuki_music_t* music, s16 volume, s16 fade) {
    if (music != 0) {
        DisableEvent(g_main_root_counter_2_event);
        music->master_volume.value = 0;
        SuzukiCalcMusVolChange(music, volume, fade);
        main_sound_set_reverb_mode(
            music->reverb_mode, music->reverb_depth, music->reverb_delay, music->reverb_feedback);
        main_smd_set_voice_attr_flags_all_channels(music, 0x7000 | SUZUKI_VOICE_ATTR_ALL);
        main_smd_retrigger_held_voices(music);
        music->status = (music->status & ~SUZUKI_MUSIC_STATUS_PAUSED) | SUZUKI_MUSIC_STATUS_PLAYING;
        EnableEvent(g_main_root_counter_2_event);
    }
}
