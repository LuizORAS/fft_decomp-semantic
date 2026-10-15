#include "fft/world.h"
#include "psx/types.h"

/* WORLD's copy of battle_script_load_next_event: yield until no thread runs an event instruction's
 * task (NATIVE_THREAD_TASK_CAMERA to NATIVE_THREAD_TASK_ADD_UNIT), then load the event
 * EVENT_SCRIPT_VAR_CURRENT_EVENT names unless it is already loaded. Returns 1 when a load happened. */
s32 world_script_load_next_event(void) {
    s32 task_id;
    s32 event_id;

    do {
        world_thread_yield();
        for (task_id = NATIVE_THREAD_TASK_CAMERA; task_id <= NATIVE_THREAD_TASK_ADD_UNIT; task_id++) {
            if (world_thread_find_running_by_task(task_id) != 0) {
                break;
            }
        }
    } while (task_id != NATIVE_THREAD_TASK_ADD_UNIT + 1);

    event_id = world_script_get_variable(EVENT_SCRIPT_VAR_CURRENT_EVENT);
    if (event_id != g_world_script_loaded_event_id) {
        world_script_load_event(event_id);
        g_world_script_loaded_event_id = event_id;
        return 1;
    }
    return 0;
}
