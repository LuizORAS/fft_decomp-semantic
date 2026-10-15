#include "fft/event_attack.h"

/* Unloads the current scenario music, then loads the two requested music
 * ids (main_music_e, 0 for none) into slots 1 and 2 through the main-stack
 * dispatcher, each followed by the one-VSync wait stub, and plays slot 1
 * when slot_1_music_id is nonzero. */
void attack_sound_play_music_tracks(s32 slot_1_music_id, s32 slot_2_music_id) {
    main_sound_unload_scenario_music_and_stop_tune();

    if (slot_1_music_id != 0) {
        g_battle_thread_call_target = (void (*)(void))main_sound_load_music_into_slot;
        battle_thread_call_on_main_stack(slot_1_music_id, 1);
        attack_sound_wait_for_music_stub();
    }

    if (slot_2_music_id != 0) {
        g_battle_thread_call_target = (void (*)(void))main_sound_load_music_into_slot;
        battle_thread_call_on_main_stack(slot_2_music_id, 2);
        attack_sound_wait_for_music_stub();
    }

    if (slot_1_music_id != 0) {
        g_battle_thread_call_target = (void (*)(void))main_sound_switch_music_track;
        battle_thread_call_on_main_stack(1, 0x7f, 0);
    }
}
