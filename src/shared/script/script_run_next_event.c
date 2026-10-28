#include "fft/script.h"

/* Body of the event thread: load the event the current-event variable names, then
 * interpret it. script_start_current_event_thread (world_script_start_current_event_thread in WORLD)
 * starts it on thread slot 1. BATTLE and WORLD each compile this text twice; nothing starts the second
 * copy (script_run_next_event_unused). */
void script_run_next_event(void) {
    script_load_next_event();
    script_execute_event();
}
