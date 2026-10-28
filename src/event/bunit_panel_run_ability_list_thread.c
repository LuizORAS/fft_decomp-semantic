#include "fft/event_bunit.h"
#include "fft/thread.h"

/* BUNIT's stand-in for equip_panel_run_ability_list_thread: yields once and exits. Nothing starts it. */
void bunit_panel_run_ability_list_thread(void) {
    thread_yield();
    thread_exit_current();
}
