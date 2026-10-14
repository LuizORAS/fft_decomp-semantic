#include "fft/main.h"
#include "psx/types.h"

/* Blocking form of main_sound_start_music_load_into_free_slot: polls the loader each
 * frame until the music record exists. Returns the slot, or 0 when no load
 * started. */
s32 main_sound_wait_for_music_load(s32 music_id) {
    s32 result;

    result = main_sound_start_music_load_into_free_slot(music_id);
    if (result != 0) {
        while (main_sound_poll_music_load() != 0) {
            VSync(0);
            main_system_frame_hook();
            main_file_poll_load(&g_main_file_cd_state);
        }
    }
    return result;
}
