#include "fft/event_debugchr.h"
#include "fft/thread.h"

/* DEBUGCHR's stand-in for equip_panel_run_equipment_list_thread: yields once and exits. Nothing starts it. */
void debugchr_panel_run_equipment_list_thread(void) {
    thread_yield();
    thread_exit_current();
}
