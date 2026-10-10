#include "fft/wldcore.h"
#include "psx/types.h"

void wldcore_sound_change_scenario_music(s32 music_id) {
    s16 packed = g_wldcore_active_saved_record.music_selection;
    s32 slot = packed >> 8;

    if ((packed & 0xFF) != 0) {
        main_sound_stop_current_music();
    }
    if (slot != 0) {
        main_sound_unload_scenario_mus(slot);
    } else {
        slot = 2;
    }
    VSync(2);
    main_sound_open_and_play_music(music_id, slot);
    g_wldcore_active_saved_record.music_selection = (slot << 8) | music_id;
    while (main_sound_is_music_loading_stub() != 0) {
        VSync(0);
    }
    g_wldcore_audio_queue.music.tracks[slot] = music_id;
    g_wldcore_audio_queue.current_slot = slot;
}
