#include "fft/battle_text.h"
#include "fft/menu.h"
#include "fft/thread.h"

/* Starts the text character handling thread on thread_id with the given
 * parameters when no text is pending. Returns 0 once the pending text
 * matches `key` and has finished, 1 when a new thread was started, otherwise
 * the running status. */
s32 text_start_thread_if_idle(s32 thread_id, s32 parameter_0, s32 key, s32 parameter_2, s32 y_offset) {
    s32 status = thread_is_running(thread_id);

    if (status == 0) {
        if (g_menu_text_active_key == key) {
            g_menu_text_active_key = 0;
            return 0;
        }
    }
    if (g_menu_text_active_key == 0) {
        thread_start(thread_id, battle_text_run_dialog_thread);
        thread_set_parameters(thread_id, parameter_0, key, parameter_2);
        g_menu_text_active_key = key;
        g_threads[thread_id].task_words[3] = y_offset;
        return 1;
    }
    return status;
}
