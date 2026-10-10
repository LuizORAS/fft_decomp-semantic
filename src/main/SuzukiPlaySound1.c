#include "fft/main.h"
#include "psx/types.h"

/* Plays a sound on SFX channels 6-7 at the default volume (0x60) and centre
 * balance, with a priority main_sound_find_sfx_channel never steals. */
void SuzukiPlaySound1(s32 sound) {
    if ((g_main_sound_driver_flags & MAIN_SOUND_DRIVER_SFX_ENABLED) != 0) {
        g_main_sound_sfx_channel_count = 2;
        main_sound_start_sfx(MAIN_SOUND_SFX_CHANNEL_ID(MAIN_SOUND_SFX_PRIORITY_DIRECT, 6), sound, 0x6000, 0x4000);
    }
}
