#include "fft/battle_menu.h"
#include "fft/menu.h"
#include "fft/thread.h"

/* Open BUNIT.OUT through a child thread and wait for its transition to finish. WORLD compiles this
 * text too. */
void menu_open_bunit(void) {
    battle_menu_set_modal_open();
    thread_start(g_thread_current_id - 1, battle_menu_run_bunit_transition_thread);
    thread_wait_until_inactive(g_thread_current_id - 1);
    thread_exit_current();
}
