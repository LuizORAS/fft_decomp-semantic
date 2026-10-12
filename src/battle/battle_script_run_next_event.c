#include "fft/battle_event.h"

/* Body of the event thread: load the event the current-event variable names, then
 * interpret it. battle_script_start_current_event_thread starts it on thread slot
 * 1. */
void battle_script_run_next_event(void) {
    battle_script_load_next_event();
    battle_script_execute_event();
}
