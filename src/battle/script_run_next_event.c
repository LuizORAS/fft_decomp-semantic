#include "fft/battle_event.h"

/* Body of the event thread: load the event the current-event variable names, then
 * interpret it. script_start_current_event_thread starts it on thread slot
 * 1. */
void script_run_next_event(void) {
    script_load_next_event();
    script_execute_event();
}
