#include "fft/world.h"

/* Open BUNIT.OUT through a child thread and wait for its transition to finish. */
void world_menu_open_bunit(void) {
    world_menu_set_transition_active_flag();
    world_thread_start(g_world_thread_current_id - 1, world_menu_run_bunit_transition_thread);
    world_thread_wait_until_inactive(g_world_thread_current_id - 1);
    world_thread_exit_current();
}
