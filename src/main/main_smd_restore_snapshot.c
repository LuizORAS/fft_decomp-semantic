#include "fft/main.h"
#include "psx/libapi.h"
#include "psx/libetc.h"
#include "psx/types.h"

/* Restores a MUS record from its saved snapshot: queues the voices playing
 * now for key-off, copies the snapshot back, keeps the current sequence_ticks as
 * snapshot_restore_tick, forces a full voice update and retriggers the held notes. */
void main_smd_restore_snapshot(suzuki_music_t* music) {
    s32 tick;

    if (music->snapshot != 0 && (music->status & SUZUKI_MUSIC_STATUS_SNAPSHOT) != 0) {
        DisableEvent(g_main_root_counter_2_event);
        g_main_sound_music_key_off_voices |= SuzukiGetActiveChannels(music);
        tick = music->sequence_ticks;
        main_smd_copy_snapshot(music, music->snapshot);
        music->snapshot_restore_tick = tick;
        main_smd_set_voice_attr_flags_all_channels(music, 0x71ff);
        music->key_off_mask = 0;
        music->key_on_mask = 0;
        main_smd_retrigger_held_voices(music);
        EnableEvent(g_main_root_counter_2_event);
    }
}
