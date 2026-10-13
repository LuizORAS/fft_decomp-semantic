#include "fft/thread.h"
#include "fft/world.h"

/* Open BUNIT.OUT through a child thread and wait for its transition to finish. */
void world_menu_open_bunit(void) {
    world_menu_set_transition_active_flag();
    thread_start(g_thread_current_id - 1, world_menu_run_bunit_transition_thread);
    thread_wait_until_inactive(g_thread_current_id - 1);
    thread_exit_current();
}
