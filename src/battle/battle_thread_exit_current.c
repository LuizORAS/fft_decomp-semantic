#include "fft/battle.h"
#include "psx/types.h"

/* Stop the current thread (clear its running flag and task id) and yield; it runs again only
 * if its slot is resumed or restarted. */
void battle_thread_exit_current(void) {
    g_battle_threads[g_battle_thread_current_id].is_running = 0;
    g_battle_threads[g_battle_thread_current_id].task_id = 0;
    battle_thread_yield();
}
