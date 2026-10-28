#include "psx/suzuki.h"
#include "psx/types.h"

/* Same as suzuki_sound_play_sfx, with explicit volume and balance bytes; SMD
 * opcode 0x9C (suzuki_smd_play_sfx) passes balance 0x40 (center). */
void suzuki_sound_play_sfx_with_settings_find_channel(s32 sound_id, s32 volume, s32 balance) {
    if ((g_suzuki_sound_driver_flags & MAIN_SUZUKI_SOUND_DRIVER_SFX_ENABLED) != 0) {
        s32 channel = suzuki_sound_find_sfx_channel(sound_id, 2);
        g_suzuki_sound_sfx_channel_count = 2;
        suzuki_sound_start_sfx(MAIN_SUZUKI_SOUND_SFX_CHANNEL_ID(MAIN_SUZUKI_SOUND_SFX_PRIORITY_STEALABLE, channel),
            sound_id, volume << 8, balance << 8);
    }
}
