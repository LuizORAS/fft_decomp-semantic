#include "fft/main.h"
#include "psx/types.h"

/* Plays a sound on SFX channels 4-5, like SuzukiPlaySound1 on 6-7. */
void SuzukiPlaySound2(s32 sound) {
    if ((g_main_sound_driver_flags & MAIN_SOUND_DRIVER_SFX_ENABLED) != 0) {
        g_main_sound_sfx_channel_count = 2;
        main_sound_start_sfx(MAIN_SOUND_SFX_CHANNEL_ID(MAIN_SOUND_SFX_PRIORITY_BACKGROUND, 4), sound, 0x6000, 0x4000);
    }
}
