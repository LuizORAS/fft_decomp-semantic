#include "fft/main.h"
#include "psx/libspu.h"
#include "psx/types.h"

/* Selects the output sound type and reapplies the volumes, reverb and CD mix
 * that depend on it.
 *
 * Type 0 (mono) and any value outside 1-3 leave the status type bits clear;
 * 1 (stereo) sets 0x100, 2 (wide) 0x300 and 3 0x500 (no menu offers it).
 * The explicit case 0 makes the target's compare tree (root at case 1). */
void main_sound_set_type(s32 type) {
    suzuki_music_t* music;

    g_main_sound_driver_flags &= ~(MAIN_SOUND_DRIVER_WIDE_OPPOSITE | MAIN_SOUND_DRIVER_WIDE | MAIN_SOUND_DRIVER_STEREO);
    switch (type) {
    case MAIN_SOUND_TYPE_MONO:
        break;
    case MAIN_SOUND_TYPE_STEREO:
        g_main_sound_driver_flags |= MAIN_SOUND_DRIVER_STEREO;
        break;
    case MAIN_SOUND_TYPE_WIDE:
        g_main_sound_driver_flags |= MAIN_SOUND_DRIVER_WIDE | MAIN_SOUND_DRIVER_STEREO;
        break;
    case MAIN_SOUND_TYPE_WIDE_OPPOSITE:
        g_main_sound_driver_flags |= MAIN_SOUND_DRIVER_WIDE_OPPOSITE | MAIN_SOUND_DRIVER_STEREO;
        break;
    }
    main_sound_commit_volume_change();
    SpuSetReverbModeParam(&g_main_sound_reverb_attr);
    g_main_sound_reverb_attr.mask = 0;
    for (music = g_main_sound_active_music_list; music != 0; music = music->next) {
        main_smd_set_voice_update_flags_all_channels(SUZUKI_VOICE_UPDATE_VOLUME, music);
    }
    if (g_main_sound_driver_flags & MAIN_SOUND_DRIVER_PUT_TYPE) {
        main_sound_put_type(g_main_sound_spu_state.cd_mix_level);
    }
}
