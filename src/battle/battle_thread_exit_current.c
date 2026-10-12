#include "fft/battle_thread.h"

/* End the current thread: mark its slot idle with no task, then yield. */
void battle_thread_exit_current(void) {
    g_battle_threads[g_battle_current_thread_id].is_running = 0;
    g_battle_threads[g_battle_current_thread_id].task_id = NATIVE_THREAD_TASK_NONE;
    battle_thread_yield();
}
