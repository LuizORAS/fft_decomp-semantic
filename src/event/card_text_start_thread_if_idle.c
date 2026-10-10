#include "fft/event_card.h"
#include "psx/types.h"

/* Starts the text character handling thread on thread_id with the given
 * parameters when no text is pending. Returns 0 once the pending text
 * matches `key` and has finished, 1 when a new thread was started, otherwise
 * the running status. */
s32 card_text_start_thread_if_idle(s32 thread_id, s32 parameter_0, s32 key, s32 parameter_2, s32 thread_field_5c) {
    s32 status = battle_thread_is_running(thread_id);

    if (status == 0) {
        if (g_card_thread_active_key == key) {
            g_card_thread_active_key = 0;
            return 0;
        }
    }
    if (g_card_thread_active_key == 0) {
        battle_thread_start(thread_id, battle_text_character_handling_thread);
        battle_thread_set_parameters(thread_id, parameter_0, key, parameter_2);
        g_card_thread_active_key = key;
        g_battle_threads[thread_id].task_words[3] = thread_field_5c;
        return 1;
    }
    return status;
}
