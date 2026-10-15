#include "fft/main.h"
#include "psx/types.h"

/* Plays a sound effect on the SFX voice pair that starts at the even
 * channel at or below channel. */
void main_sound_play_sfx_in_channel(s32 sound_id, s32 channel) {
    channel &= 0xfffe;
    if ((g_main_sound_driver_flags & MAIN_SOUND_DRIVER_SFX_ENABLED) != 0) {
        g_main_sound_sfx_channel_count = 2;
        main_sound_start_sfx(
            MAIN_SOUND_SFX_CHANNEL_ID(MAIN_SOUND_SFX_PRIORITY_STEALABLE, channel), sound_id, 0x6000, 0x4000);
    }
}
