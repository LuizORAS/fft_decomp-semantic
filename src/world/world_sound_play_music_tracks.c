#include "fft/world.h"
#include "psx/types.h"

/* Unloads the current scenario music, then loads the two requested music
 * ids (main_music_e, 0 for none) into slots 1 and 2 through the main-stack
 * dispatcher, waiting for each load to finish, and plays slot 1. */
void world_sound_play_music_tracks(s32 slot_1_music_id, s32 slot_2_music_id) {
    main_sound_unload_scenario_music_and_tunes();
    if (slot_1_music_id != 0) {
        g_world_thread_call_target = (void (*)(void))main_sound_open_and_play_music;
        world_thread_call_on_main_stack(slot_1_music_id, 1);
        world_sound_wait_for_music_stub();
    }
    if (slot_2_music_id != 0) {
        g_world_thread_call_target = (void (*)(void))main_sound_open_and_play_music;
        world_thread_call_on_main_stack(slot_2_music_id, 2);
        world_sound_wait_for_music_stub();
    }
    if (slot_1_music_id != 0) {
        g_world_thread_call_target = (void (*)(void))main_sound_switch_music_track;
        world_thread_call_on_main_stack(1, 0x7f, 0);
    }
}
