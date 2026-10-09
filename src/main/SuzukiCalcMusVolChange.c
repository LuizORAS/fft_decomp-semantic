#include "fft/main.h"
#include "psx/libapi.h"
#include "psx/libetc.h"
#include "psx/types.h"

/* Sets the music's master volume, immediately when time is 0, otherwise as
 * a ramp over time ticks. A non-zero volume also resumes music paused by
 * main_smd_pause_music, like main_smd_resume_music. */
void SuzukiCalcMusVolChange(suzuki_music_t* music, s16 volume, s16 time) {
    s32 difference;

    music->master_volume.target = volume << 8;
    if (time == 0) {
        music->master_volume.value = volume << 24;
        music->master_volume.count = 0;
        main_smd_set_voice_update_flags_all_channels(SUZUKI_VOICE_UPDATE_VOLUME, music);
    } else {
        difference = (volume << 16) - (music->master_volume.value >> 8);
        if (difference == 0) {
            return;
        }
        music->master_volume.count = time;
        music->master_volume.step = (difference / time) << 8;
    }
    if ((music->status & SUZUKI_MUSIC_STATUS_PAUSED) && volume != 0) {
        DisableEvent(g_main_root_counter_2_event);
        main_sound_set_reverb_mode(
            music->reverb_mode, music->reverb_depth, music->reverb_delay, music->reverb_feedback);
        main_smd_set_voice_attr_flags_all_channels(music, 0x7000 | SUZUKI_VOICE_ATTR_ALL);
        main_smd_retrigger_held_voices(music);
        music->status = (music->status & ~SUZUKI_MUSIC_STATUS_PAUSED) | SUZUKI_MUSIC_STATUS_PLAYING;
        EnableEvent(g_main_root_counter_2_event);
    }
}
