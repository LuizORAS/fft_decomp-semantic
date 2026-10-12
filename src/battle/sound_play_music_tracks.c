#include "fft/battle_sound.h"
#include "fft/battle_thread.h"
#include "psx/types.h"

/* BATTLE's copy of attack_sound_play_music_tracks (ATTACK.OUT 0x801c3e60):
 * loads the two music ids (main_music_e, 0 for none) into slots 1 and 2 and
 * plays slot 1. */
void sound_play_music_tracks(s32 slot_1_music_id, s32 slot_2_music_id) {
    main_sound_unload_scenario_music_and_tunes();

    if (slot_1_music_id != 0) {
        g_battle_thread_call_target = (void (*)(void))main_sound_open_and_play_music;
        battle_thread_call_on_main_stack(slot_1_music_id, 1);
        sound_wait_for_music_stub();
    }

    if (slot_2_music_id != 0) {
        g_battle_thread_call_target = (void (*)(void))main_sound_open_and_play_music;
        battle_thread_call_on_main_stack(slot_2_music_id, 2);
        sound_wait_for_music_stub();
    }

    if (slot_1_music_id != 0) {
        g_battle_thread_call_target = (void (*)(void))main_sound_switch_music_track;
        battle_thread_call_on_main_stack(1, 0x7f, 0);
    }
}
