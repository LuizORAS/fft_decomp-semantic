#include "fft/script.h"
#include "fft/thread.h"

/* Yield until no thread runs an event instruction's task (THREAD_TASK_CAMERA to
 * THREAD_TASK_ADD_UNIT), then load the event EVENT_SCRIPT_VAR_CURRENT_EVENT names
 * (script_load_event) unless it is the one already loaded. Returns 1 when it loaded a new
 * block, 0 otherwise. BATTLE and WORLD compile this text. */
s32 script_load_next_event(void) {
    s32 task_id;
    s32 event_id;

    do {
        thread_yield();
        for (task_id = THREAD_TASK_CAMERA; task_id <= THREAD_TASK_ADD_UNIT; task_id++) {
            if (battle_thread_find_running_by_task(task_id) != 0) {
                break;
            }
        }
    } while (task_id != THREAD_TASK_ADD_UNIT + 1);

    event_id = script_get_variable(EVENT_SCRIPT_VAR_CURRENT_EVENT);
    if (event_id == g_script_loaded_event_id) {
        return 0;
    }
    script_load_event(event_id);
    g_script_loaded_event_id = event_id;
    return 1;
}
