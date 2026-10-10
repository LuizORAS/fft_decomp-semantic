#include "fft/event_attack.h"

void attack_sound_play_music_tracks(s32 slot_1_music_id, s32 slot_2_music_id) {
    main_sound_unload_scenario_music_and_tunes();

    if (slot_1_music_id != 0) {
        g_battle_thread_call_target = (void (*)(void))main_sound_open_and_play_music;
        battle_thread_call_on_main_stack(slot_1_music_id, 1);
        attack_sound_wait_for_music_stub();
    }

    if (slot_2_music_id != 0) {
        g_battle_thread_call_target = (void (*)(void))main_sound_open_and_play_music;
        battle_thread_call_on_main_stack(slot_2_music_id, 2);
        attack_sound_wait_for_music_stub();
    }

    if (slot_1_music_id != 0) {
        g_battle_thread_call_target = (void (*)(void))main_sound_switch_music_track;
        battle_thread_call_on_main_stack(1, 0x7f, 0);
    }
}
